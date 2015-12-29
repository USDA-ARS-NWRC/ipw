/*
 ** orchestrate computation of horizon
 **/

#include <math.h>

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"


fpixel_t *hor1d(
		PARM_T parm,		/* parameter structure */
		fpixel_t *zbuf		/* elevation buffer		 */
)
{
	fpixel_t        fspace;		/* (fpixel_t)spacing		 */
	fpixel_t        thresh = 0;	/* threshold for mask		 */
	fpixel_t       *hcos = NULL;	/* horizon cosines 		 */
//	fpixel_t       *zbuf;		/* elevation buffer		 */
//	int             ngot;		/* # pixels read		 */
	int             N;		/* # pixels in row		 */
	int            *hbuf;		/* horizon index		 */
	fpixel_t        *hmask = NULL;	/* output mask			 */
	int 			i;			/* loop index */


	/*
	 *	routine for forward or backward horz calculation
	 */
	int  (*horfun)(int n, fpixel_t *z, int *h);

	/*
	 * initialize buffers for i/o and horizon computation
	 */
	N = hnsamps(parm.i_fd) * hnlines(parm.i_fd);

	/* NOSTRICT */
	hbuf = (int *) ecalloc(N, sizeof(int));
	if (hbuf == NULL || zbuf == NULL) {
		bug("buffer allocation");
	}

	/*
	 * if horizon cosines to be computed, need to allocate storage vector
	 */
	if (parm.nbits > 1) {
		/* NOSTRICT */
		hcos = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
		if (hcos == NULL)
			bug("buffer allocation");
	}

	/*
	 * otherwise if mask to be computed, need to allocate integer storage
	 * vector and set threshold
	 */
	else {
		/* NOSTRICT */
		hmask = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
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
	for (i = 0; i < N; i++) {

		/*
		 * find points that form horizons
		 */
		(void) (*horfun) (i, zbuf, hbuf);

		/*
		 * if not mask output, compute and write horizons along each row
		 */
		if (parm.nbits > 1) {
			horval(i, zbuf, fspace, hbuf, hcos);
		}

		/*
		 * if mask output, set mask and write
		 */
		else {
			hormask(i, zbuf, fspace, hbuf, thresh, hmask);
		}
	}


//	SAFE_FREE(zbuf);
	SAFE_FREE(hbuf);
	if (parm.nbits > 1) {
//		SAFE_FREE(hcos);
		return hcos;
	} else {
//		SAFE_FREE(hmask);
		return hmask;
	}
}
