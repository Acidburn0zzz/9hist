#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"

enum {
	Width		= 160,
	Height		= 25,

	Attr		= 7,		/* white on black */
};

#define CGASCREENBASE	((uchar*)KADDR(0xB8000))

static int cgapos;
static int screeninitdone;
static Lock cgascreenlock;

static uchar
cgaregr(int index)
{
	outb(0x3D4, index);
	return inb(0x3D4+1) & 0xFF;
}

static void
cgaregw(int index, int data)
{
	outb(0x3D4, index);
	outb(0x3D4+1, data);
}

static void
movecursor(void)
{
	cgaregw(0x0E, (cgapos/2>>8) & 0xFF);
	cgaregw(0x0F, cgapos/2 & 0xFF);
	CGASCREENBASE[cgapos+1] = Attr;
}

static void
cgascreenputc(int c)
{
	int i;

	if(c == '\n'){
		cgapos = cgapos/Width;
		cgapos = (cgapos+1)*Width;
	}
	else if(c == '\t'){
		i = 8 - ((cgapos/2)&7);
		while(i-->0)
			cgascreenputc(' ');
	}
	else if(c == '\b'){
		if(cgapos >= 2)
			cgapos -= 2;
		cgascreenputc(' ');
		cgapos -= 2;
	}
	else{
		CGASCREENBASE[cgapos++] = c;
		CGASCREENBASE[cgapos++] = Attr;
	}
	if(cgapos >= Width*Height){
		memmove(CGASCREENBASE, &CGASCREENBASE[Width], Width*(Height-1));
		memset(&CGASCREENBASE[Width*(Height-1)], 0, Width);
		cgapos = Width*(Height-1);
	}
	movecursor();
}

void
screeninit(void)
{
	cgapos = cgaregr(0x0E)<<8;
	cgapos |= cgaregr(0x0F);
	cgapos *= 2;
	screeninitdone = 1;
}

static void
cgascreenputs(char* s, int n)
{
	if(!islo()){
		/*
		 * Don't deadlock trying to
		 * print in an interrupt.
		 */
		if(!canlock(&cgascreenlock))
			return;
	}
	else
		lock(&cgascreenlock);

	while(n-- > 0)
		cgascreenputc(*s++);

	unlock(&cgascreenlock);
}

void (*screenputs)(char*, int) = cgascreenputs;
