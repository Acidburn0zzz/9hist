#undef	CHDIR	/* BUG */
#include "/sys/src/libc/9syscall/sys.h"

typedef long Syscall(ulong*);

Syscall sysr1;
Syscall syserrstr;
Syscall sysbind;
Syscall syschdir;
Syscall sysclose;
Syscall sysdup;
Syscall sysalarm;
Syscall sysexec;
Syscall sysexits;
Syscall sysfork;
Syscall sysforkpgrp;
Syscall sysfstat;
Syscall syssegbrk;
Syscall sysmount;
Syscall sysopen;
Syscall sysread;
Syscall sysseek;
Syscall syssleep;
Syscall sysstat;
Syscall sys__wait__;
Syscall syswrite;
Syscall syspipe;
Syscall syscreate;
Syscall sysrfork;
Syscall sysbrk_;
Syscall sysremove;
Syscall syswstat;
Syscall sysfwstat;
Syscall sysnotify;
Syscall sysnoted;
Syscall syssegattach;
Syscall syssegdetach;
Syscall syssegfree;
Syscall syssegflush;
Syscall sysrendezvous;
Syscall sysunmount;
Syscall syswait;

Syscall *systab[]={
	[SYSR1]		sysr1,
	[ERRSTR]	syserrstr,
	[BIND]		sysbind,
	[CHDIR]		syschdir,
	[CLOSE]		sysclose,
	[DUP]		sysdup,
	[ALARM]		sysalarm,
	[EXEC]		sysexec,
	[EXITS]		sysexits,
	[FORK]		sysfork,
	[FORKPGRP]	sysforkpgrp,
	[FSTAT]		sysfstat,
	[SEGBRK]	syssegbrk,
	[MOUNT]		sysmount,
	[OPEN]		sysopen,
	[READ]		sysread,
	[SEEK]		sysseek,
	[SLEEP]		syssleep,
	[STAT]		sysstat,
	[__WAIT__]	sys__wait__,
	[WRITE]		syswrite,
	[PIPE]		syspipe,
	[CREATE]	syscreate,
	[RFORK]		sysrfork,
	[BRK_]		sysbrk_,
	[REMOVE]	sysremove,
	[WSTAT]		syswstat,
	[FWSTAT]	sysfwstat,
	[NOTIFY]	sysnotify,
	[NOTED]		sysnoted,
	[SEGATTACH]	syssegattach,
	[SEGDETACH]	syssegdetach,
	[SEGFREE]	syssegfree,
	[SEGFLUSH]	syssegflush,
	[RENDEZVOUS]	sysrendezvous,
	[UNMOUNT]	sysunmount,
	[WAIT]		syswait,
};

char *sysctab[]={
	[SYSR1]		"Running",
	[ERRSTR]	"Errstr",
	[BIND]		"Bind",
	[CHDIR]		"Chdir",
	[CLOSE]		"Close",
	[DUP]		"Dup",
	[ALARM]		"Alarm",
	[EXEC]		"Exec",
	[EXITS]		"Exits",
	[FORK]		"Fork",
	[FORKPGRP]	"Forkpgrp",
	[FSTAT]		"Fstat",
	[SEGBRK]	"Segbrk",
	[MOUNT]		"Mount",
	[OPEN]		"Open",
	[READ]		"Read",
	[SEEK]		"Seek",
	[SLEEP]		"Sleep",
	[STAT]		"Stat",
	[__WAIT__]	"__wait_",
	[WRITE]		"Write",
	[PIPE]		"Pipe",
	[CREATE]	"Create",
	[RFORK]		"Rfork",
	[BRK_]		"Brk",
	[REMOVE]	"Remove",
	[WSTAT]		"Wstat",
	[FWSTAT]	"Fwstat",
	[NOTIFY]	"Notify",
	[NOTED]		"Noted",
	[SEGATTACH]	"Segattach",
	[SEGDETACH]	"Segdetach",
	[SEGFREE]	"Segfree",
	[SEGFLUSH]	"Segflush",
	[RENDEZVOUS]	"Rendez",
	[UNMOUNT]	"Unmount",
	[WAIT]		"Wait",
};
