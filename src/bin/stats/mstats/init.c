#include "ipw.h"

#include "pgm.h"

/*
 * init -- initialize dynamic data structures
 */

void
init(void)
{
 /* NOSTRICT */
	parm.i_buf = (fpixel_t *) ecalloc(parm.nsamps * parm.i_nbands,
					  sizeof(fpixel_t));
	if (parm.i_buf == NULL) {
		error("can't allocate input buffer");
	}

 /* NOSTRICT */
	parm.c_buf = (pixel_t *) ecalloc(parm.nsamps, sizeof(pixel_t));
	if (parm.c_buf == NULL) {
		error("can't allocate input buffer");
	}

 /* NOSTRICT */
	parm.c_npixels = (int *) ecalloc(parm.c_nclasses, sizeof(int));
	if (parm.c_npixels == NULL) {
		error("can't allocate pixel tally buffer");
	}

 /* NOSTRICT */
	parm.sum_x = (double **) allocnd(sizeof(double), 2,
					 parm.c_nclasses, parm.i_nbands);
	if (parm.sum_x == NULL) {
		error("can't allocate sum-of-band buffer");
	}
 /*
  * NB:  Only the lower triangle of the array parm.sum_xy is used.  If space
  * becomes a problem for images with humongous # bands, then we may want to
  * store this array in lower-triangular form ...
  */
 /* NOSTRICT */
	parm.sum_xy = (double ***) allocnd(sizeof(double), 3,
					   parm.c_nclasses,
					   parm.i_nbands, parm.i_nbands);
	if (parm.sum_xy == NULL) {
		error("can't allocate sum-of-band*band buffer");
	}
}
