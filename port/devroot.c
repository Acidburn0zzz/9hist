#include	"u.h"
#include	"../port/lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"../port/error.h"

enum{
	Qdir=	0,
	Qrecover,

	Nfiles=32,	/* max root files */
};

extern ulong	bootlen;
extern uchar	bootcode[];

Dirtab rootdir[Nfiles]=
{
	"recover",	{Qrecover},	0,	0777,
};

static uchar	*rootdata[Nfiles];
static int	nroot = 1;
static int	recovbusy;

typedef struct Recover Recover;
struct Recover
{
	int	len;
	char	*req;
	Recover	*next;
};

struct
{
	Lock;
	QLock;
	Rendez;
	Recover	*q;
}reclist;

/*
 *  add a root file
 */
static void
addroot(char *name, uchar *contents, ulong len, int perm)
{
	Dirtab *d;

	if(nroot >= Nfiles)
		panic("too many root files");
	rootdata[nroot] = contents;
	d = &rootdir[nroot];
	strcpy(d->name, name);
	d->length = len;
	d->perm = perm;
	d->qid.path = nroot+1;
	if(perm & CHDIR)
		d->qid.path |= CHDIR;
	nroot++;
}

/*
 *  add a root file
 */
void
addrootfile(char *name, uchar *contents, ulong len)
{
	addroot(name, contents, len, 0555);
}

/*
 *  add a root file
 */
static void
addrootdir(char *name)
{
	addroot(name, nil, 0, CHDIR);
}

static void
rootreset(void)
{
	addrootdir("bin");
	addrootdir("dev");
	addrootdir("env");
	addrootdir("proc");
	addrootdir("net");
	addrootdir("net.alt");
	addrootdir("root");
	addrootdir("srv");
	addrootdir("xsrv");

	addrootfile("boot", bootcode, bootlen);	/* always have a boot file */
}

static Chan*
rootattach(char *spec)
{
	return devattach('/', spec);
}

static int
rootwalk(Chan *c, char *name)
{
	if(strcmp(name, "..") == 0) {
		c->qid.path = Qdir|CHDIR;
		return 1;
	}
	if((c->qid.path & ~CHDIR) != Qdir)
		return 0;
	return devwalk(c, name, rootdir, nroot, devgen);
}

static void
rootstat(Chan *c, char *dp)
{
	devstat(c, dp, rootdir, nroot, devgen);
}

static Chan*
rootopen(Chan *c, int omode)
{
	switch(c->qid.path & ~CHDIR) {
	default:
		break;
	case Qrecover:
		if(recovbusy)
			error(Einuse);
		if(strcmp(up->user, eve) != 0)
			error(Eperm);
		recovbusy = 1;
		break;
	}

	return devopen(c, omode, rootdir, nroot, devgen);
}

/*
 * sysremove() knows this is a nop
 */
static void
rootclose(Chan *c)
{
	switch(c->qid.path) {
	default:
		break;
	case Qrecover:
		if(c->flag&COPEN)
			recovbusy = 0;
		break;
	}
}

static int
rdrdy(void*)
{
	return reclist.q != 0;
}

static long
rootread(Chan *c, void *buf, long n, vlong off)
{
	ulong t;
	Dirtab *d;
	uchar *data;
	Recover *r;
	ulong offset = off;

	t = c->qid.path & ~CHDIR;
	switch(t){
	case Qdir:
		return devdirread(c, buf, n, rootdir, nroot, devgen);
	case Qrecover:
		qlock(&reclist);
		if(waserror()) {
			qunlock(&reclist);
			nexterror();
		}

		sleep(&reclist, rdrdy, 0);

		lock(&reclist);
		r = reclist.q;
		reclist.q = r->next;
		unlock(&reclist);

		qunlock(&reclist);

		poperror();
		if(n < r->len)
			n = r->len;
		memmove(buf, r->req, n);
		free(r->req);
		free(r);
		return n;
	}

	d = &rootdir[t-1];
	data = rootdata[t-1];
	if(offset >= d->length)
		return 0;
	if(offset+n > d->length)
		n = d->length - offset;
	memmove(buf, data+offset, n);
	return n;
}

static long
rootwrite(Chan *c, void *buf, long n, vlong)
{
	char tmp[256];

	switch(c->qid.path & ~CHDIR){
	default:
		error(Egreg);
	case Qrecover:
		if(n > sizeof(tmp)-1)
			error(Etoosmall);
		/* Nul terminate */
		memmove(tmp, buf, n);
		tmp[n] = '\0';
		mntrepl(tmp);
		return n;
	}
	return 0;
}

static void
rootcreate(Chan *c, char *name, int mode, ulong perm)
{
	if(!iseve() || c->qid.path != (CHDIR|Qdir) ||
	   (perm & CHDIR) == 0 || mode != OREAD)
		error(Eperm);
	addrootdir(name);
	c->flag |= COPEN;
	c->mode = OREAD;
}

Dev rootdevtab = {
	'/',
	"root",

	rootreset,
	devinit,
	rootattach,
	devclone,
	rootwalk,
	rootstat,
	rootopen,
	rootcreate,
	rootclose,
	rootread,
	devbread,
	rootwrite,
	devbwrite,
	devremove,
	devwstat,
};

void
rootrecover(Chan *c, char *mntname)
{
	Recover *r;

	r = malloc(sizeof(Recover));
	r->req = smalloc(c->name->len+strlen(mntname)+2);
	sprint(r->req, "%s %s", c->name->s, mntname);
	lock(&reclist);
	r->next = reclist.q;
	reclist.q = r;
	unlock(&reclist);
	wakeup(&reclist);
}
