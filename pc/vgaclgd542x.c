#include "u.h"
#include "../port/lib.h"
#include "mem.h"
#include "dat.h"
#include "fns.h"
#include "../port/error.h"

#include <libg.h>
#include "screen.h"
#include "vga.h"

static Lock clgd542xlock;

static void
clgd542xpage(int page)
{
	lock(&clgd542xlock);
	vgaxo(Grx, 0x09, page<<4);
	unlock(&clgd542xlock);
}

static Vgac clgd542x = {
	"clgd542x",
	clgd542xpage,

	0,
};

void
vgaclgd542xlink(void)
{
	addvgaclink(&clgd542x);
}
