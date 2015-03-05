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
	int             band;		/* current band #		 */

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	parm.nlines = bih_nlines(i_bihpp[0]);
	parm.nsamps = bih_nsamps(i_bihpp[0]);
	parm.nbands = bih_nbands(i_bihpp[0]);
	parm.nbits = bih_nbits(i_bihpp[0]);
 /*
  * may not have > 8 bits / pixel
  */
	if (parm.nbits > 8) {
		uferr(parm.i_fd);
		error("%d-bit pixels (must be <= 8)", parm.nbits);
	}
 /*
  * may have either 1 or 3 bands; all bands must have same # bits / pixel
  */
	if (parm.nbands != 1 && parm.nbands != 3) {
		error("%d bands (must be either 1 or 3)", parm.nbands);
	}

	for (band = 1; band < parm.nbands; ++band) {
		if (bih_nbits(i_bihpp[band]) != parm.nbits) {
			error("pixel sizes differ: bands 0, %d", band);
		}
	}
 /*
  * skip remaining headers
  */
	skiphdrs(parm.i_fd);
}
