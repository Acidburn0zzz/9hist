#include	"u.h"
#include	"../port/lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"

/*
 *  task state segment.  Plan 9 ignores all the task switching goo and just
 *  uses the tss for esp0 and ss0 on gate's into the kernel, interrupts,
 *  and exceptions.  The rest is completely ignored.
 *
 *  This means that we only need one tss in the whole system.
 */
typedef struct Tss	Tss;
struct Tss
{
	ulong	backlink;	/* unused */
	ulong	sp0;		/* pl0 stack pointer */
	ulong	ss0;		/* pl0 stack selector */
	ulong	sp1;		/* pl1 stack pointer */
	ulong	ss1;		/* pl1 stack selector */
	ulong	sp2;		/* pl2 stack pointer */
	ulong	ss2;		/* pl2 stack selector */
	ulong	cr3;		/* page table descriptor */
	ulong	eip;		/* instruction pointer */
	ulong	eflags;		/* processor flags */
	ulong	eax;		/* general (hah?) registers */
	ulong 	ecx;
	ulong	edx;
	ulong	ebx;
	ulong	esp;
	ulong	ebp;
	ulong	esi;
	ulong	edi;
	ulong	es;		/* segment selectors */
	ulong	cs;
	ulong	ss;
	ulong	ds;
	ulong	fs;
	ulong	gs;
	ulong	ldt;		/* local descriptor table */
	ulong	iomap;		/* io map base */
};
Tss tss;

/*
 *  segment descriptor initializers
 */
#define	DATASEGM(p) 	{ 0xFFFF, SEGG|SEGB|(0xF<<16)|SEGP|SEGPL(p)|SEGDATA|SEGW }
#define	EXECSEGM(p) 	{ 0xFFFF, SEGG|SEGD|(0xF<<16)|SEGP|SEGPL(p)|SEGEXEC|SEGR }
#define CALLGATE(s,o,p)	{ ((o)&0xFFFF)|((s)<<16), (o)&0xFFFF0000|SEGP|SEGPL(p)|SEGCG }
#define	D16SEGM(p) 	{ 0xFFFF, (0x0<<16)|SEGP|SEGPL(p)|SEGDATA|SEGW }
#define	E16SEGM(p) 	{ 0xFFFF, (0x0<<16)|SEGP|SEGPL(p)|SEGEXEC|SEGR }
#define	TSSSEGM(b,p)	{ ((b)<<16)|sizeof(Tss),\
			  ((b)&0xFF000000)|(((b)>>16)&0xFF)|SEGTSS|SEGPL(p)|SEGP }

/*
 *  global descriptor table describing all segments
 */
Segdesc gdt[] =
{
[NULLSEG]	{ 0, 0},		/* null descriptor */
[KDSEG]		DATASEGM(0),		/* kernel data/stack */
[KESEG]		EXECSEGM(0),		/* kernel code */
[UDSEG]		DATASEGM(3),		/* user data/stack */
[UESEG]		EXECSEGM(3),		/* user code */
[TSSSEG]	TSSSEGM(0,0),		/* tss segment */
};

static Page	ktoppg;		/* prototype top level page table
				 * containing kernel mappings  */
static ulong	*kpt;		/* 2nd level page tables for kernel mem */

#define ROUNDUP(s,v)	(((s)+(v-1))&~(v-1))
/*
 *  offset of virtual address into
 *  top level page table
 */
#define TOPOFF(v)	(((ulong)(v))>>(2*PGSHIFT-2))

/*
 *  offset of virtual address into
 *  bottom level page table
 */
#define BTMOFF(v)	((((ulong)(v))>>(PGSHIFT))&(WD2PG-1))

#define MAXUMEG 64	/* maximum memory per user process in megabytes */
#define ONEMEG (1024*1024)

struct
{
	Lock;
	ulong addr; 	/* next available address for isa bus memory */
	ulong end;	/* one past available isa bus memory */
} isamemalloc;

/*
 *  Change current page table and the stack to use for exceptions
 *  (traps & interrupts).  The exception stack comes from the tss.
 *  Since we use only one tss, (we hope) there's no need for a
 *  puttr().
 */
static void
taskswitch(ulong pagetbl, ulong stack)
{
	tss.ss0 = KDSEL;
	tss.sp0 = stack;
tss.ss1 = KDSEL;
tss.sp1 = stack;
tss.ss2 = KDSEL;
tss.sp2 = stack;
	tss.cr3 = pagetbl;
	putcr3(pagetbl);
}

/*
 *  Create a prototype page map that maps all of memory into
 *  kernel (KZERO) space.  This is the default map.  It is used
 *  whenever the processor is not running a process or whenever running
 *  a process which does not yet have its own map.
 */
void
mmuinit(void)
{
	int i, nkpt, npage, nbytes;
	ulong x;
	ulong y;
	ulong *top;

	/*
	 *  set up the global descriptor table. we make the tss entry here
	 *  since it requires arithmetic on an address and hence cannot
	 *  be a compile or link time constant.
	 */
	x = (ulong)&tss;
	gdt[TSSSEG].d0 = (x<<16)|sizeof(Tss);
	gdt[TSSSEG].d1 = (x&0xFF000000)|((x>>16)&0xFF)|SEGTSS|SEGPL(0)|SEGP;
	putgdt(gdt, sizeof gdt);

	/*
	 *  set up system page tables.
	 *  map all of physical memory to start at KZERO.
	 *  leave a map entry for a user area.
	 */

	/*  allocate top level table */
	top = xspanalloc(BY2PG, BY2PG, 0);
	ktoppg.va = (ulong)top;
	ktoppg.pa = ktoppg.va & ~KZERO;

	/*  map all memory to KZERO (add some address space for ISA memory) */
	isamemalloc.addr = conf.topofmem;
	isamemalloc.end = conf.topofmem + ISAMEMSIZE;
	if(isamemalloc.end > 64*MB)
		isamemalloc.end = 64*MB;	/* ISA can only access 64 meg */
	npage = isamemalloc.end/BY2PG;
	nbytes = PGROUND(npage*BY2WD);		/* words of page map */
	nkpt = nbytes/BY2PG;			/* pages of page map */
	kpt = xspanalloc(nbytes, BY2PG, 0);
	for(i = 0; i < npage; i++)
		kpt[i] = (0+i*BY2PG) | PTEVALID | PTEKERNEL | PTEWRITE;
	x = TOPOFF(KZERO);
	y = ((ulong)kpt)&~KZERO;
	for(i = 0; i < nkpt; i++)
		top[x+i] = (y+i*BY2PG) | PTEVALID | PTEKERNEL | PTEWRITE;

	/*
	 *  set up the task segment
	 */
	memset(&tss, 0, sizeof(tss));
	taskswitch(ktoppg.pa, BY2PG + (ulong)m);
	puttr(TSSSEL);/**/
}

/*
 *  Mark the mmu and tlb as inconsistent and call mmuswitch to fix it up.
 */
void
flushmmu(void)
{
	int s;

	s = splhi();
	up->newtlb = 1;
	mmuswitch(up);
	splx(s);
}

/*
 *  Switch to a process's memory map.  If the process doesn't
 *  have a map yet, just use the prototype one that contains
 *  mappings for only the kernel.
 */
void
mmuswitch(Proc *p)
{
	Page *pg;
	ulong *top;

	if(p->newtlb){
		/*
		 *  newtlb set means that they are inconsistent
		 *  with the segment.c data structures.
		 *
		 *  bin the current second level page tables and
		 *  the pointers to them in the top level page.
		 *  pg->daddr is used by putmmu to save the offset into
		 *  the top level page.
		 */
		if(p->mmutop && p->mmuused){
			top = (ulong*)p->mmutop->va;
			for(pg = p->mmuused; pg->next; pg = pg->next)
				top[pg->daddr] = 0;
			top[pg->daddr] = 0;
			pg->next = p->mmufree;
			p->mmufree = p->mmuused;
			p->mmuused = 0;
		}
		p->newtlb = 0;
	}

	/* tell processor about new page table (flushes cached entries) */
	if(p->mmutop)
		taskswitch(p->mmutop->pa, (ulong)(p->kstack+KSTACK));
	else
		taskswitch(ktoppg.pa, (ulong)(p->kstack+KSTACK));
}

static void
simpleputpage(Page *pg)
{
	if(pg->ref != 1)
		panic("simpleputpage");

	pg->ref = 0;
	if(palloc.head){
		pg->next = palloc.head;
		palloc.head->prev = pg;
	}
	else {
		palloc.tail = pg;
		pg->next = 0;
	}
	palloc.head = pg;
	pg->prev = 0;

	palloc.freecount++;
	if(palloc.r.p != 0)
		wakeup(&palloc.r);
}

/*
 *  give all page table pages back to the free pool.  This is called in sched()
 *  with palloc locked.
 */
void
mmurelease(Proc *p)
{
	Page *pg;
	Page *next;

	/* point 386 to protoype page map and m->stack */
	taskswitch(ktoppg.pa, BY2PG + (ulong)m);

	/* give away page table pages */
	for(pg = p->mmufree; pg; pg = next){
		next = pg->next;
		simpleputpage(pg);
	}
	p->mmufree = 0;
	for(pg = p->mmuused; pg; pg = next){
		next = pg->next;
		simpleputpage(pg);
	}
	p->mmuused = 0;
	if(p->mmutop)
		simpleputpage(p->mmutop);
	p->mmutop = 0;
}

/*
 *  Add an entry into the mmu.
 */
void
putmmu(ulong va, ulong pa, Page *pg)
{
	int topoff;
	ulong *top;
	ulong *pt;
	int s;

	/*
	 *  create a top level page if we don't already have one.
	 *  copy the kernel top level page into it for kernel mappings.
	 */
	if(up->mmutop == 0){
		pg = newpage(0, 0, 0);
		pg->va = VA(kmap(pg));
		memmove((void*)pg->va, (void*)ktoppg.va, BY2PG);
		up->mmutop = pg;
	}
	top = (ulong*)up->mmutop->va;
	topoff = TOPOFF(va);

	/*
	 *  if bottom level page table missing, allocate one 
	 *  and point the top level page at it.
	 */
	s = splhi();
	if(PPN(top[topoff]) == 0){
		if(up->mmufree == 0){
			spllo();
			pg = newpage(1, 0, 0);
			pg->va = VA(kmap(pg));
			splhi();
		} else {
			pg = up->mmufree;
			up->mmufree = pg->next;
			memset((void*)pg->va, 0, BY2PG);
		}
		top[topoff] = PPN(pg->pa) | PTEVALID | PTEUSER | PTEWRITE;
		pg->daddr = topoff;
		pg->next = up->mmuused;
		up->mmuused = pg;
	}

	/*
	 *  put in new mmu entry
	 */
	pt = (ulong*)(PPN(top[topoff])|KZERO);
	pt[BTMOFF(va)] = pa | PTEUSER;

	/* flush cached mmu entries */
	/*taskswitch(up->mmutop->pa, (ulong)(up->kstack+KSTACK));/**/
	putcr3(up->mmutop->pa);/**/
	splx(s);
}

/*
 *  allocate some address space (already mapped into the kernel)
 *  for ISA bus memory.
 */
ulong
isamem(int len)
{
	ulong a, x;

	lock(&isamemalloc);
	len = PGROUND(len);
	x = isamemalloc.addr + len;
	if(x > isamemalloc.end)
		panic("isamem");
	a = isamemalloc.addr;
	isamemalloc.addr = x;
	unlock(&isamemalloc);
	return a;
}

/*
 *  used to map a page into 16 meg - BY2PG for confinit(). tpt is the temporary
 *  page table set up by l.s.
 */
long*
mapaddr(ulong addr)
{
	ulong base;
	ulong off;
	static ulong *pte, top;
	extern ulong tpt[];

	if(pte == 0){
		top = (((ulong)tpt)+(BY2PG-1))&~(BY2PG-1);
		pte = (ulong*)top;
		top &= ~KZERO;
		top += BY2PG;
		pte += (4*1024*1024-BY2PG)>>PGSHIFT;
	}

	base = off = addr;
	base &= ~(KZERO|(BY2PG-1));
	off &= BY2PG-1;

	*pte = base|PTEVALID|PTEKERNEL|PTEWRITE; /**/
	putcr3((ulong)top);

	return (long*)(KZERO | 4*1024*1024-BY2PG | off);
}
