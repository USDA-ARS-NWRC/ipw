/*
** orchestrate computation of horizon
**/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

void
horizon(void)
{
	fpixel_t        fspace;		/* (fpixel_t)spacing		 */
	fpixel_t        thresh = 0;	/* threshold for mask		 */
	fpixel_t       *hcos = NULL;	/* horizon cosines 		 */
	fpixel_t       *zbuf;		/* elevation buffer		 */
	int             ngot;		/* # pixels read		 */
	int             npix;		/* # pixels in row		 */
	int            *hbuf;		/* horizon index		 */
	pixel_t        *hmask = NULL;	/* output mask			 */


/*
 *	routine for forward or backward horz calculation
 */
	int  (*horfun)(int n, fpixel_t *z, int *h);

 /*
  * initialize buffers for i/o and horizon computation
  */
	npix = hnsamps(parm.i_fd);
 /* NOSTRICT */
	hbuf = (int *) ecalloc(npix, sizeof(int));
 /* NOSTRICT */
	zbuf = (fpixel_t *) ecalloc(npix, sizeof(fpixel_t));
	if (hbuf == NULL || zbuf == NULL)
		bug("buffer allocation");
 /*
  * if horizon cosines to be computed, need to allocate storage vector
  */
	if (parm.nbits > 1) {
 /* NOSTRICT */
		hcos = (fpixel_t *) ecalloc(npix, sizeof(fpixel_t));
		if (hcos == NULL)
			bug("buffer allocation");
	}
 /*
  * otherwise if mask to be computed, need to allocate integer storage
  * vector and set threshold
  */
	else {
 /* NOSTRICT */
		hmask = (pixel_t *) ecalloc(npix, sizeof(pixel_t));
		if (hmask == NULL)
			bug("buffer allocation");
		thresh = tan(M_PI_2 - parm.zenith);
	}
 /*
  * forward or backward horizon function
  */
	horfun = (parm.backward) ? hor1b : hor1f;
 /*
  * main loop
  */
	fspace = parm.spacing;
	while ((ngot = fpvread(parm.i_fd, zbuf, npix)) > 0) {
		if (ngot != npix)
			error("premature end of row");
 /*
  * find points that form horizons
  */
		(void) (*horfun) (ngot, zbuf, hbuf);
 /*
  * if not mask output, compute and write horizons along each row
  */
		if (parm.nbits > 1) {
			horval(ngot, zbuf, fspace, hbuf, hcos);
			if (fpvwrite(parm.o_fd, hcos, ngot) != ngot) {
				error("fpvwrite error");
			}
		}
 /*
  * if mask output, set mask and write
  */
		else {
			hormask(ngot, zbuf, fspace, hbuf, thresh, hmask);
			if (pvwrite(parm.o_fd, hmask, ngot) != ngot) {
				error("pvwrite error");
			}
		}
	}

	SAFE_FREE(zbuf);
	SAFE_FREE(hbuf);
	if (parm.nbits > 1) {
		SAFE_FREE(hcos);
	} else {
		SAFE_FREE(hmask);
	}
}
