#include	"u.h"
#include	"lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"

#include	<libg.h>
#include	<gnot.h>
#include	"screen.h"

enum {
	Data=		0x60,	/* data port */

	Status=		0x64,	/* status port */
	 Inready=	0x01,	/*  input character ready */
	 Outbusy=	0x02,	/*  output busy */
	 Sysflag=	0x04,	/*  system flag */
	 Cmddata=	0x08,	/*  cmd==0, data==1 */
	 Inhibit=	0x10,	/*  keyboard/mouse inhibited */
	 Minready=	0x20,	/*  mouse character ready */
	 Rtimeout=	0x40,	/*  general timeout */
	 Parity=	0x80,

	Cmd=		0x64,	/* command port (write only) */

	Spec=	0x80,

	PF=	Spec|0x20,	/* num pad function key */
	View=	Spec|0x00,	/* view (shift window up) */
	KF=	Spec|0x40,	/* function key */
	Shift=	Spec|0x60,
	Break=	Spec|0x61,
	Ctrl=	Spec|0x62,
	Latin=	Spec|0x63,
	Caps=	Spec|0x64,
	Num=	Spec|0x65,
	Middle=	Spec|0x66,
	No=	Spec|0x7F,	/* no mapping */

	Home=	KF|13,
	Up=	KF|14,
	Pgup=	KF|15,
	Print=	KF|16,
	Left=	View,
	Right=	View,
	End=	'\r',
	Down=	View,
	Pgdown=	View,
	Ins=	KF|20,
	Del=	0x7F,
};

uchar kbtab[] = 
{
[0x00]	No,	0x1b,	'1',	'2',	'3',	'4',	'5',	'6',
[0x08]	'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
[0x10]	'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
[0x18]	'o',	'p',	'[',	']',	'\n',	Ctrl,	'a',	's',
[0x20]	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
[0x28]	'\'',	'`',	Shift,	'\\',	'z',	'x',	'c',	'v',
[0x30]	'b',	'n',	'm',	',',	'.',	'/',	Shift,	No,
[0x38]	Latin,	' ',	Ctrl,	KF|1,	KF|2,	KF|3,	KF|4,	KF|5,
[0x40]	KF|6,	KF|7,	KF|8,	KF|9,	KF|10,	Num,	KF|12,	Home,
[0x48]	No,	No,	No,	No,	No,	No,	No,	No,
[0x50]	No,	No,	No,	No,	No,	No,	No,	KF|11,
[0x58]	KF|12,	No,	No,	No,	No,	No,	No,	No,
};

uchar kbtabshift[] =
{
[0x00]	No,	0x1b,	'!',	'@',	'#',	'$',	'%',	'^',
[0x08]	'&',	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
[0x10]	'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
[0x18]	'O',	'P',	'{',	'}',	'\n',	Ctrl,	'A',	'S',
[0x20]	'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
[0x28]	'"',	'~',	Shift,	'|',	'Z',	'X',	'C',	'V',
[0x30]	'B',	'N',	'M',	'<',	'>',	'?',	Shift,	No,
[0x38]	Latin,	' ',	Ctrl,	KF|1,	KF|2,	KF|3,	KF|4,	KF|5,
[0x40]	KF|6,	KF|7,	KF|8,	KF|9,	KF|10,	Num,	KF|12,	Home,
[0x48]	No,	No,	No,	No,	No,	No,	No,	No,
[0x50]	No,	No,	No,	No,	No,	No,	No,	KF|11,
[0x58]	KF|12,	No,	No,	No,	No,	No,	No,	No,
};

uchar kbtabesc1[] =
{
[0x00]	No,	No,	No,	No,	No,	No,	No,	No,
[0x08]	No,	No,	No,	No,	No,	No,	No,	No,
[0x10]	No,	No,	No,	No,	No,	No,	No,	No,
[0x18]	No,	No,	No,	No,	No,	Ctrl,	No,	No,
[0x20]	No,	No,	No,	No,	No,	No,	No,	No,
[0x28]	No,	No,	No,	No,	No,	No,	No,	No,
[0x30]	No,	No,	No,	No,	No,	No,	No,	Print,
[0x38]	Latin,	No,	No,	No,	No,	No,	No,	No,
[0x40]	No,	No,	No,	No,	No,	No,	Break,	Home,
[0x48]	Up,	Pgup,	No,	Down,	No,	Right,	No,	End,
[0x50]	Left,	Pgdown,	Ins,	Del,	No,	No,	No,	No,
[0x58]	No,	No,	No,	No,	No,	No,	No,	No,
};

/*
 *  keyboard input q
 */
KIOQ	kbdq;

static int mousebuttons;
static int middlebutton;

/*
 *  predeclared
 */
static void	kbdintr(Ureg*);


/*
 *  wait for output no longer busy
 */
static int
outready(void)
{
	int tries;

	for(tries = 0; (inb(Status) & Outbusy); tries++)
		if(tries > 1000)
			return -1;
	return 0;
}

/*
 *  wait for input
 */
static int
inready(void)
{
	int tries;

	for(tries = 0; !(inb(Status) & Inready); tries++)
		if(tries > 1000)
			return -1;
	return 0;
}

/*
 *  send a command to the mouse
 */
static int
mousecmd(int cmd)
{
	unsigned int c;
	int tries;

	c = 0;
	do{
		for(tries=0; tries < 10; tries++){
			if(outready() < 0)
				return -1;
			outb(Cmd, 0xD4);
			if(outready() < 0)
				return -1;
			outb(Data, cmd);
			if(inready() < 0)
				return -1;
			c = inb(Data);
		}
	} while(c == 0xFE);
	if(c != 0xFA)
		return -1;
	return 0;
}

void
kbdinit(void)
{
	int c, s;
	int tries;

	initq(&kbdq);
	setvec(Kbdvec, kbdintr);
	initq(&mouseq);
	setvec(Mousevec, kbdintr);

	/* wait for a quiescent controller */
	while((c = inb(Status)) & (Outbusy | Inready))
		if(c & Inready)
			inb(Data);

	/* enable kbd/mouse xfers and interrupts */
	outb(Cmd, 0x60);
	if(outready() < 0)
		print("kbd init failed\n");
	outb(Data, 0x47);
	if(outready() < 0)
		print("kbd init failed\n");
	outb(Cmd, 0xA8);

	/* make mouse streaming, enabled */
	if(mousecmd(0xEA) < 0
	|| mousecmd(0xF4) < 0)
		print("can't initialize mouse\n");
}

/*
 *  mouse message is three bytes
 *
 *	byte 0 -	0 0 SDY SDX 1 M R L
 *	byte 1 -	DX
 *	byte 2 -	DY
 */
static void
mymouseputc(int c)
{
	static short msg[3];
	static int nb;
	static uchar b[] = {0, 1, 4, 5, 2, 3, 6, 7};
	static lastdx, lastdy;
	int diff;
	extern Mouseinfo mouse;

	/* 
	 *  check byte 0 for consistency
	 */
	if(nb==0 && (c&0xc8)!=0x08)
		return;

	msg[nb] = c;
	if(++nb == 3){
		nb = 0;
		if(msg[0] & 0x10)
			msg[1] |= 0xFF00;
		if(msg[0] & 0x20)
			msg[2] |= 0xFF00;

		mousebuttons = b[msg[0]&7];
		mouse.newbuttons = mousebuttons | middlebutton;
		mouse.dx = msg[1];
		mouse.dy = -msg[2];
		mouse.track = 1;
		spllo();		/* mouse tracking kills uart0 */
		mouseclock();
	}
}

/*
 *  Ctrl key used as middle button pressed
 */
static void
middle(int newval)
{
	middlebutton = newval;
	mouse.newbuttons = mousebuttons | middlebutton;
	mouse.dx = 0;
	mouse.dy = 0;
	mouse.track = 1;
	spllo();		/* mouse tracking kills uart0 */
	mouseclock();
}

/*
 *  keyboard interrupt
 */
int
kbdintr0(void)
{
	int s, c, nc;
	static int esc1, esc2;
	static int shift;
	static int caps;
	static int ctl;
	static int num;
	static int lstate, k1, k2;
	int keyup;

	/*
	 *  get status
	 */
	s = inb(Status);
	if(!(s&Inready))
		return -1;

	/*
	 *  get the character
	 */
	c = inb(Data);

	/*
	 *  if it's the mouse...
	 */
	if(s & Minready){
		mymouseputc(c);
		return 0;
	}

	/*
	 *  e0's is the first of a 2 character sequence
	 */
	if(c == 0xe0){
		esc1 = 1;
		return 0;
	} else if(c == 0xe1){
		esc2 = 2;
		return 0;
	}

	keyup = c&0x80;
	c &= 0x7f;
	if(c > sizeof kbtab){
		print("unknown key %ux\n", c|keyup);
		kbdputc(&kbdq, k1);
		return 0;
	}

	if(esc1){
		c = kbtabesc1[c];
		esc1 = 0;
		if(!keyup)
			kbdputc(&kbdq, c);
		return 0;
	} else if(esc2){
		esc2--;
		return 0;
	} else if(shift)
		c = kbtabshift[c];
	else
		c = kbtab[c];

	if(caps && c<='z' && c>='a')
		c += 'A' - 'a';

	/*
	 *  keyup only important for shifts
	 */
	if(keyup){
		switch(c){
		case Shift:
			shift = 0;
			break;
		case Ctrl:
			ctl = 0;
			break;
		case Middle:
			middle(0);
			break;
		}
		return 0;
	}

	/*
 	 *  normal character
	 */
	if(!(c & Spec)){
		if(ctl)
			c &= 0x1f;
		switch(lstate){
		case 1:
			k1 = c;
			lstate = 2;
			return 0;
		case 2:
			k2 = c;
			lstate = 0;
			c = latin1(k1, k2);
			if(c == 0){
				kbdputc(&kbdq, k1);
				c = k2;
			}
			/* fall through */
		default:
			break;
		}
	} else {
		switch(c){
		case Caps:
			caps ^= 1;
			return 0;
		case Num:
			num ^= 1;
			return 0;
		case Shift:
			shift = 1;
			return 0;
		case Latin:
			lstate = 1;
			return 0;
		case Ctrl:
			ctl = 1;
			return 0;
		case Middle:
			middle(2);
			return 0;
		}
	}
	kbdputc(&kbdq, c);
	return 0;
}

void
kbdintr(Ureg *ur)
{
	while(kbdintr0() == 0)
		;
}
