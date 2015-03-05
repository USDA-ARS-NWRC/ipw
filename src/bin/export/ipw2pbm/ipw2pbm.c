#include "ipw.h"

#include "bih.h"

#include "pgm.h"

/*
 * ipw2pbm -- write PBM header, then copy pixels
 */

void
ipw2pbm(void)
{
	char            hdr[MAX_INPUT];	/* PBM header buffer		 */

 /*
  * build and write PBM header
  */
	(void) sprintf(hdr, "%s\n%d %d\n%d\n",
		       parm.nbands == 1 ? PGM_COOKIE : PPM_COOKIE,
		       parm.nsamps, parm.nlines, ipow2(parm.nbits) - 1);

	if (uputs(parm.o_fd, hdr) == ERROR) {
		error("PBM header write failed");
	}
 /*
  * copy raw pixel data (IPW storage and interleaving is same as PBM)
  */
	if (ucopy(parm.i_fd, parm.o_fd, imgsize(parm.i_fd)) == ERROR) {
		error("image data copy failed");
	}
}
