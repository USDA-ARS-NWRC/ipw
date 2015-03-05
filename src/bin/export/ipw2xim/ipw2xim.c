#include "ipw.h"

#include "bih.h"
#include "pgm.h"

void
ipw2xim(void)
{
	ximhdr();

	if (ucopy(parm.i_fd, parm.o_fd, imgsize(parm.i_fd)) == ERROR) {
		error("can't copy image data");
	}
}
