Alarm	*alarm(int, void (*)(Alarm*), void*);
void	alarminit(void);
Block	*allocb(ulong);
int	anyready(void);
void	append(List**, List*);
void	cacheinit(void);
void	cancel(Alarm*);
int	canlock(Lock*);
int	canqlock(QLock*);
void	chaninit(void);
void	chandevreset(void);
void	chandevinit(void);
void	checkalarms(void);
#define	clearmmucache()
void	clock(Ureg*);
void	clockinit(void);
Chan	*clone(Chan*, Chan*);
void	close(Chan*);
void	closemount(Mount*);
void	closepgrp(Pgrp*);
long	clrfpintr(void);
int	compactpte(Orig*, ulong);
void	confinit(void);
Env	*copyenv(Env*, int);
Chan	*createdir(Chan*);
int	decref(Ref*);
void	delay(int);
void	delete(List**, List*, List*);
void	delete0(List**, List*);
Chan	*devattach(int, char*);
Chan	*devclone(Chan*, Chan*);
void	devdir(Chan*, Qid, char*, long, long, Dir*);
long	devdirread(Chan*, char*, long, Dirtab*, int, Devgen*);
Devgen	devgen;
int	devno(int, int);
Chan	*devopen(Chan*, int, Dirtab*, int, Devgen*);
void	devstat(Chan*, char*, Dirtab*, int, Devgen*);
int	devwalk(Chan*, char*, Dirtab*, int, Devgen*);
Chan	*domount(Chan*);
void	duartbaud(int);
void	duartbreak(int);
void	duartdtr(int);
void	duartinit(void);
void	duartintr(void);
int	duartinputport(void);
void	duartstartrs232o(void);
void	duartstarttimer(void);
void	duartstoptimer(void);
void	dumpregs(Ureg*);
void	dumpstack(void);
int	eqchan(Chan*, Chan*, long);
int	eqqid(Qid, Qid);
void	envpgclose(Env *);
void	error(int);
void	errors(char*);
void	evenaddr(ulong);
char	*excname(ulong);
void	execpc(ulong);
void	exit(void);
int	fault(ulong, int);
void	faultasync(Ureg*);
void	fdclose(int);
Chan	*fdtochan(int, int);
void	filsys(Chan*, char*, long);
void	filsysinit(void);
void	firmware(void);
void	flowctl(Queue*);
void	flushcpucache(void);
void	flushmmu(void);
void	forkmod(Seg*, Seg*, Proc*);
int	fpcr(int);
void	fpsave(FPsave*);
void	fpregsave(char*);
void	fprestore(FPsave*);
void	fpregrestore(char*);
void	freeb(Block*);
int	freebroken(void);
void	freenextmod(PTE*);
void	freepage(Orig*, int);
void	freepte(Orig*);
void	freesegs(int);
void	freealarm(Alarm*);
Block	*getb(Blist*);
int	getb2(ulong);
int	getfields(char*, char**, int, char);
Block	*getq(Queue*);
int	getrs232o(void);
int	getw2(ulong);
void	gotolabel(Label*);
void	growpte(Orig*, ulong);
void	*ialloc(ulong, int);
int	incref(Ref*);
void	insert(List**, List*, List*);
void	intrinit(void);
void	invalidateu(void);
void	isdir(Chan*);
void	kbdchar(int);
void	kbdrepeat(int);
void	kbdclock(void);
void	kmapinit(void);
KMap	*kmap(Page*);
KMap	*kmappa(ulong, ulong);
int	kprint(char*, ...);
void	kproc(char*, void(*)(void*), void*);
void	kunmap(KMap*);
void	lanceintr(void);
void	lancesetup(Lance*);
void	lock(Lock*);
void	lockinit(void);
Orig	*lookorig(ulong, ulong, int, Chan*);
void	machinit(void);
void	mapstack(Proc*);
void	mmuinit(void);
int	mount(Chan*, Chan*, int);
void	mousebuttons(int);
void	mousechar(int);
void	mouseclock(void);
Chan	*namec(char*, int, int, ulong);
void	nexterror(void);
Alarm	*newalarm(void);
Chan	*newchan(void);
PTE	*newmod(Orig *o);
Mount	*newmount(void);
Orig	*neworig(ulong, ulong, int, Chan*);
Page	*newpage(int, Orig*, ulong);
Pgrp	*newpgrp(void);
Proc	*newproc(void);
void	newqinfo(Qinfo*);
char	*nextelem(char*, char*);
int	nonetcksum(Block*, int);
void	nonetfreeifc(Noifc*);
Noifc*	nonetnewifc(Queue*, Stream*, int, int, int, void (*)(Noconv*, char*));
void	nonetrcvmsg(Noconv*, Block*);
void	notify(Ureg*);
void	nullput(Queue*, Block*);
int	openmode(ulong);
Block	*padb(Block*, int);
void	pageinit(void);
void	panic(char*, ...);
void	pexit(char*, int);
void	pgrpcpy(Pgrp*, Pgrp*);
void	pgrpinit(void);
void	pgrpnote(Pgrp*, char*, long, int);
Pgrp	*pgrptab(int);
int	postnote(Proc*, int, char*, int);
int	pprint(char*, ...);
void	printinit(void);
void	printslave(void);
void	procinit0(void);
void	procrestore(Proc*, uchar*);
void	procsave(uchar*, int);
void	procsetup(Proc*);
Proc	*proctab(int);
Queue	*pushq(Stream*, Qinfo*);
int	putb(Blist*, Block*);
void	putb2(ulong, int);
void	putbq(Blist*, Block*);
void	putcontext(int);
void	putcxreg(int);
void	putcxsegm(int, ulong, int);
void	putmmu(ulong, ulong);
void	putpmeg(ulong, ulong);
int	putq(Queue*, Block*);
void	putsegm(ulong, int);
void	putstr(char*);
void	putstrn(char*, long);
void	puttbr(ulong);
void	putw2(ulong, ulong);
void	putw4(ulong, ulong);
void	putwC(ulong, ulong);
void	putwD(ulong, ulong);
void	putwD16(ulong, ulong);
void	putwE(ulong, ulong);
void	putwE16(ulong, ulong);
ulong	pwait(Waitmsg*);
int	readnum(ulong, char*, ulong, ulong, int);
void	ready(Proc*);
void	reset(void);
int	return0(void*);
void	rs232ichar(int);
Proc	*runproc(void);
void	qlock(QLock*);
void	qunlock(QLock*);
void	restartprint(Alarm*);
void	restfpregs(FPsave*);
void	savefpregs(FPsave*);
void	sched(void);
void	schedinit(void);
void	screeninit(void);
void	screenputc(int);
long	seconds(void);
Seg	*seg(Proc*, ulong);
int	segaddr(Seg*, ulong, ulong);
void	serviceinit(void);
void	service(char*, Chan*, Chan*, void (*)(Chan*, char*, long));
int	setlabel(Label*);
char	*skipslash(char*);
void	sleep(Rendez*, int(*)(void*), void*);
int	splhi(void);
int	spllo(void);
void	splx(int);
void	spldone(void);
ulong	swap1(ulong*);
Devgen	streamgen;
int	streamclose(Chan*);
int	streamclose1(Stream*);
int	streamenter(Stream*);
int	streamexit(Stream*, int);
void	streaminit(void);
long	streamread(Chan*, void*, long);
long	streamwrite(Chan*, void*, long, int);
Stream	*streamnew(ushort, ushort, ushort, Qinfo*, int);
void	streamopen(Chan*, Qinfo*);
int	streamparse(char*, Block*);
void	streamstat(Chan*, char*, char*);
long	stringread(Chan*, void*, long, char*);
long	syscall(Ureg*);
int	tas(char*);
void	touser(ulong);
void	trap(Ureg*);
void	trapinit(void);
void	tsleep(Rendez*, int (*)(void*), void*, int);
void	twakeme(Alarm*);
long	unionread(Chan*, void*, long);
void	unlock(Lock*);
void	usepage(Page*, int);
void	unusepage(Page*, int);
void	userinit(void);
void	validaddr(ulong, ulong, int);
void	*vmemchr(void*, int, int);
void	wakeme(Alarm*);
void	wakeup(Rendez*);
Chan	*walk(Chan*, char*, int);

#define	waserror()	(u->nerrlab++, setlabel(&u->errlab[u->nerrlab-1]))
#define	poperror()	u->nerrlab--

#define USED(x) if(x)
#define SET(x) x = 0
#define	wbflush()	/* mips compatibility */
