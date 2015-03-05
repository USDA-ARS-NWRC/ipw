#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * headers -- process input image headers
 */

void
headers(void)
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	int             nbits;		/* # bits / pixel		 */

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}
 /*
  * load image parameters from BIH
  */
	parm.nlines = bih_nlines(i_bihpp[0]);
	parm.nsamps = bih_nsamps(i_bihpp[0]);

	parm.nbands = bih_nbands(i_bihpp[0]);
	if (!(parm.nbands == 1
#ifdef XIM_COLOR
	      || parm.nbands == 3
#endif
	      )) {
		uferr(parm.i_fd);
#ifdef XIM_COLOR
		error("image must have either 1 or 3 bands");
#else
		error("image must have 1 band");
#endif
	}

	nbits = bih_nbits(i_bihpp[0]);
#ifdef XIM_COLOR
	if (parm.nbands == 3) {
		int             band;

		for (band = 1; band < 3; ++band) {
			if (bih_nbits(i_bihpp[band]) != nbits) {
				uferr(parm.i_fd);
				error(
				 "bands 0 and %d have different size pixels",
				      band);
			}
		}
	}
#endif
	parm.nlevels = ipow2(nbits);
	if (parm.nlevels > OUT_NLEVELS) {
		uferr(parm.i_fd);
		error("band 0 has more than %d levels per pixel", OUT_NLEVELS);
	}
 /*
  * skip remaining headers
  */
	skiphdrs(parm.i_fd);
}
