#include	"u.h"
#include	"lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"ureg.h"
#include	"io.h"

int faulting;

void
fault386(Ureg *ur)
{
	ulong addr;
	int read;
	int user;
	int n;
	int insyscall;

	insyscall = u->p->insyscall;
	u->p->insyscall = 1;
	addr = getcr2();
	read = !(ur->ecode & 2);
	user = (ur->cs&0xffff) == UESEL;
	n = fault(addr, read);
	if(n < 0){
		if(user){
			pprint("user %s error addr=0x%lux\n", read?"read":"write", addr);
			pprint("status=0x%lux pc=0x%lux sp=0x%lux\n", ur->flags,
				ur->pc, ur->usp);
			pexit("Suicide", 0);
		}
		dumpregs(ur);
		panic("fault: 0x%lux", addr);
	}
	u->p->insyscall = insyscall;
}

void
faultinit(void)
{
	setvec(Faultvec, fault386);
}
