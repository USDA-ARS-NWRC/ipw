#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(void)
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	if (bih_nbands(i_bihpp[0]) != 1) {
		uferr(parm.i_fd);
		error("input image must be single-band");
	}

	if (bih_nbits(i_bihpp[0]) != 1) {
		uferr(parm.i_fd);
		error("input image must have 1-bit pixels");
	}

	parm.i_nlines = bih_nlines(i_bihpp[0]);
	parm.i_nsamps = bih_nsamps(i_bihpp[0]);
 /*
  * skip remaining headers
  */
	skiphdrs(parm.i_fd);
}
