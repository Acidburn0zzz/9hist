void		accounttime(void);
void		addrootfile(char*, uchar*, ulong);
void		alarmkproc(void*);
Block*		allocb(int);
int		anyhigher(void);
int		anyready(void);
Image*		attachimage(int, Chan*, ulong, ulong);
long		authcheck(Chan*, char*, int);
void		authclose(Chan*);
long		authentread(Chan*, char*, int);
long		authentwrite(Chan*, char*, int);
long		authread(Chan*, char*, int);
void		authreply(Session*, ulong, Fcall*);
ulong		authrequest(Session*, Fcall*);
long		authwrite(Chan*, char*, int);
Page*		auxpage(void);
int		blocklen(Block*);
void		cachedel(Image*, ulong);
void		cachepage(Page*, Image*);
int		cangetc(void*);
int		canlock(Lock*);
int		canpage(Proc*);
int		canputc(void*);
int		canqlock(QLock*);
void		chandevinit(void);
void		chandevreset(void);
void		chanfree(Chan*);
void		chanrec(Mnt*);
void		checkalarms(void);
void		cinit(void);
Chan*		cclone(Chan*, Chan*);
void		cclose(Chan*);
void		closeegrp(Egrp*);
void		closefgrp(Fgrp*);
void		closemount(Mount*);
void		closepgrp(Pgrp*);
void		closergrp(Rgrp*);
long		clrfpintr(void);
int		cmount(Chan*, Chan*, int, char*);
void		confinit(void);
void		confinit1(int);
int		consactive(void);
void		(*consdebug)(void);
void		copen(Chan*);
Block*		concatblock(Block*);
Block*		copyblock(Block*, int);
void		copypage(Page*, Page*);
int		cread(Chan*, uchar*, int, ulong);
void		cunmount(Chan*, Chan*);
void		cupdate(Chan*, uchar*, int, ulong);
void		cwrite(Chan*, uchar*, int, ulong);
ulong		dbgpc(Proc*);
int		decref(Ref*);
int		decrypt(void*, void*, int);
void		delay(int);
void		devreset(void);
void		devinit(void);
Chan*		devattach(int, char*);
Block*		devbread(Chan*, long, ulong);
long		devbwrite(Chan*, Block*, ulong);
Chan*		devclone(Chan*, Chan*);
void		devcreate(Chan*, char*, int, ulong);
void		devdir(Chan*, Qid, char*, long, char*, long, Dir*);
long		devdirread(Chan*, char*, long, Dirtab*, int, Devgen*);
Devgen		devgen;
int		devno(int, int);
Chan*		devopen(Chan*, int, Dirtab*, int, Devgen*);
void		devremove(Chan*);
void		devstat(Chan*, char*, Dirtab*, int, Devgen*);
int		devwalk(Chan*, char*, Dirtab*, int, Devgen*);
void		devwstat(Chan*, char*);
Chan*		domount(Chan*);
void		dumpaproc(Proc*);
void		dumpqueues(void);
void		dumpregs(Ureg*);
void		dumpstack(void);
Fgrp*		dupfgrp(Fgrp*);
void		duppage(Page*);
void		dupswap(Page*);
int		encrypt(void*, void*, int);
void		envcpy(Egrp*, Egrp*);
int		eqchan(Chan*, Chan*, int);
int		eqqid(Qid, Qid);
void		error(char*);
long		execregs(ulong, ulong, ulong);
void		exhausted(char*);
void		exit(int);
int		fault(ulong, int);
void		fdclose(int, int);
Chan*		fdtochan(int, int, int, int);
int		fixfault(Segment*, ulong, int, int);
void		flushmmu(void);
void		forkchild(Proc*, Ureg*);
void		forkret(void);
void		free(void*);
void		freeb(Block*);
void		freeblist(Block*);
int		freebroken(void);
void		freechan(Chan*);
void		freepte(Segment*, Pte*);
void		freesegs(int);
void		freesession(Session*);
void		getcolor(ulong, ulong*, ulong*, ulong*);
void		gotolabel(Label*);
void		graphicsactive(int);
void		graphicscmap(int);
int		haswaitq(void*);
long		hostdomainwrite(char*, int);
long		hostownerwrite(char*, int);
void		iallocinit(void);
Block*		iallocb(int);
long		ibrk(ulong, int);
void		ilock(Lock*);
void		iunlock(Lock*);
int		incref(Ref*);
void		initscsi(void);
void		initseg(void);
void		isdir(Chan*);
int		iseve(void);
int		islo(void);
int		ispages(void*);
void		ixsummary(void);
void		kbdclock(void);
int		kbdcr2nl(Queue*, int);
int		kbdputc(Queue*, int);
void		kbdrepeat(int);
long		keyread(char*, int, long);
long		keywrite(char*, int);
void		kickpager(void);
void		killbig(void);
int		kprint(char*, ...);
void		kproc(char*, void(*)(void*), void*);
void		kprocchild(Proc*, void (*)(void*), void*);
void		(*kproftimer)(ulong);
void		ksetenv(char*, char*);
long		latin1(uchar*, int);
void		links(void);
void		lock(Lock*);
void		lockinit(void);
Page*		lookpage(Image*, ulong);
void		machinit(void);
void*		mallocz(ulong, int);
void*		malloc(ulong);
void		mfreeseg(Segment*, ulong, int);
void		microdelay(int);
void		mmurelease(Proc*);
void		mmuswitch(Proc*);
void		mntdump(void);
long		mntread9p(Chan*, void*, long, ulong);
void		mntrepl(char*);
long		mntwrite9p(Chan*, void*, long, ulong);
void		mountfree(Mount*);
int		msize(void*);
Chan*		namec(char*, int, int, ulong);
void		nameok(char*);
#define		nelem(x)	(sizeof(x)/sizeof(x[0]))
Chan*		newchan(void);
Mount*		newmount(Mhead*, Chan*, int, char*);
Page*		newpage(int, Segment **, ulong);
Pgrp*		newpgrp(void);
Rgrp*		newrgrp(void);
Proc*		newproc(void);
char*		nextelem(char*, char*);
void		nexterror(void);
int		notify(Ureg*);
int		nrand(int);
int		okaddr(ulong, ulong, int);
int		openmode(ulong);
Block*		padblock(Block*, int);
void		pageinit(void);
void		panic(char*, ...);
int		parseether(uchar*, char*);
int		parsefields(char*, char**, int, char*);
void		pexit(char*, int);
void		pgrpcpy(Pgrp*, Pgrp*);
void		pgrpnote(ulong, char*, long, int);
Pgrp*		pgrptab(int);
void		pio(Segment *, ulong, ulong, Page **);
#define		poperror()		up->nerrlab--
int		postnote(Proc*, int, char*, int);
int		pprint(char*, ...);
void		printinit(void);
ulong		procalarm(ulong);
int		proccounter(char *name);
void		procctl(Proc*);
void		procdump(void);
void		procinit0(void);
Proc*		proctab(int);
void		procwired(Proc*);
void		ptclone(Chan*, int, int);
void		ptclose(Pthash*);
Pte*		ptealloc(void);
Pte*		ptecpy(Pte*);
Path*		ptenter(Pthash*, Path*, char*);
int		ptpath(Path*, char*, int);
int		pullblock(Block**, int);
Block*		pullupblock(Block*, int);
void		putimage(Image*);
void		putmmu(ulong, ulong, Page*);
void		putpage(Page*);
void		putseg(Segment*);
void		putstr(char*);
void		putstr(char*);
void		putstrn(char*, int);
void		putswap(Page*);
ulong		pwait(Waitmsg*);
Block*		qbread(Queue*, int);
long		qbwrite(Queue*, Block*);
int		qcanread(Queue*);
void		qclose(Queue*);
int		qconsume(Queue*, void*, int);
Block*		qcopy(Queue*, int, ulong);
void		qdiscard(Queue*, int);
void		qflush(Queue*);
int		qfull(Queue*);
Block*		qget(Queue*);
void		qhangup(Queue*, char*);
void		qinit(void);
int		qiwrite(Queue*, void*, int);
int		qlen(Queue*);
void		qlock(QLock*);
Queue*		qopen(int, int, void (*)(void*), void*);
int		qpass(Queue*, Block*);
int		qproduce(Queue*, void*, int);
long		qread(Queue*, void*, int);
void		qreopen(Queue*);
void		qunlock(QLock*);
int		qwindow(Queue*);
int		qwrite(Queue*, void*, int);
void		qsetlimit(Queue*, int);
void		qnoblock(Queue*, int);
void		randomclock(void);
int		readnum(ulong, char*, ulong, ulong, int);
int		readstr(ulong, char*, ulong, char*);
void		ready(Proc*);
void		relocateseg(Segment*, ulong);
void		renameuser(char*, char*);
void		resched(char*);
void		resetscsi(void);
void		resrcwait(char*);
int		return0(void*);
void		rlock(RWlock*);
void		rootrecover(Path*, char*);
void		rootreq(Chan*, Mnt*);
long		rtctime(void);
void		runlock(RWlock*);
Proc*		runproc(void);
void		savefpregs(FPsave*);
void		sccclock(void);
int		sccintr(void);
void		sccsetup(void*, ulong, int);
void		sched(void);
void		scheddump(void);
void		schedinit(void);
int		screenbits(void);
#define		scsialloc(n)	mallocz((n)+512, 0)
int		scsibio(Target*, char, int, void*, long, long, long);
int		scsicap(Target*, char, ulong*, ulong*);
int		scsierrstr(int);
int		scsiexec(Target*, int, uchar*, int, void*, int*);
#define		scsifree(p)	free(p)
int		scsiinquiry(Target*, char, void*, int*);
int		scsiinv(int, int*, Target**, uchar**, char*);
int		scsireqsense(Target*, char, void*, int*, int);
int		scsistart(Target*, char, int);
int		scsitest(Target*, char);
Target*		scsiunit(int, int);
long		seconds(void);
ulong		segattach(Proc*, ulong, char *, ulong, ulong);
void		segclock(ulong);
void		segpage(Segment*, Page*);
int		setcolor(ulong, ulong, ulong, ulong);
void		setkernur(Ureg*, Proc*);
int		setlabel(Label*);
void		setregisters(Ureg*, char*, char*, int);
void		setswapchan(Chan*);
char*		skipslash(char*);
void		sleep(Rendez*, int(*)(void*), void*);
void*		smalloc(ulong);
int		splhi(void);
int		spllo(void);
void		splx(int);
void		srvrecover(Chan*, Chan*);
void		swapinit(void);
Block*		trimblock(Block*, int, int);
void		tsleep(Rendez*, int (*)(void*), void*, int);
void		unbreak(Proc*);
void		uncachepage(Page*);
long		unionread(Chan*, void*, long);
void		unlock(Lock*);
Chan*		undomount(Chan*);
void		userinit(void);
ulong		userpc(void);
long		userwrite(char*, int);
void		validaddr(ulong, ulong, int);
void		vcacheinval(Page*, ulong);
void*		vmemchr(void*, int, int);
int		wakeup(Rendez*);
Chan*		walk(Chan*, char*, int);
void		wlock(RWlock*);
void		wunlock(RWlock*);
#define		xalloc(s)	xallocz(s, 1)
void*		xallocz(ulong, int);
void		xfree(void*);
void		xhole(ulong, ulong);
void		xinit(void);
void*		xspanalloc(ulong, int, ulong);
void		xsummary(void);
Segment*	data2txt(Segment*);
Segment*	dupseg(Segment**, int, int);
Segment*	newseg(int, ulong, ulong);
Segment*	seg(Proc*, ulong, int);
void		hnputl(void*, ulong);
void		hnputs(void*, ushort);
ulong		nhgetl(void*);
ushort		nhgets(void*);
