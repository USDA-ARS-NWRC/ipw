/*
 ** orchestrate computation of horizon
 **/

#include <math.h>

#include "../horizon.new/pgm.h"
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"


void
hor1d(
		PARM_T parm,		/* parameter structure */
		fpixel_t *zbuf,		/* elevation buffer		 */
		fpixel_t *hval		/* output buffer */
)
{
	fpixel_t        fspace;			/* (fpixel_t)spacing		 */
	fpixel_t        thresh = 0;		/* threshold for mask		 */
	fpixel_t       *hcos = NULL;	/* horizon cosines 		 */
	//	fpixel_t       *zbuf;			/* elevation buffer		 */
	//	int             ngot;			/* # pixels read		 */
	int             N;				/* # pixels in row		 */
	//	int            *hbuf;			/* horizon index		 */
	fpixel_t        *hmask = NULL;	/* output mask			 */
	int 			i,j;			/* loop index */
	int 			nsamps;			/* number of samples */
	int 			nlines;			/* number of lines */


	/*
	 *	routine for forward or backward horz calculation
	 */
	int  (*horfun)(int n, fpixel_t *z, int *h);

	/*
	 * initialize buffers for i/o and horizon computation
	 */
	nsamps = hnsamps(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	N =  nsamps * nlines;

	/* NOSTRICT */
	//	hbuf = (int *) ecalloc(N, sizeof(int));
	//	if (hbuf == NULL || zbuf == NULL) {
	//		bug("buffer allocation");
	//	}

	int *hbuf; // array of pointers
	hbuf = (int *) ecalloc(nsamps, sizeof(int));

	fpixel_t *lbuf[nlines];
	fpixel_t *obuf[nlines];
	for(i = 0; i < nlines; i++) {
		lbuf[i] = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
		obuf[i] = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
		for(j = 0; j < nsamps; j++) {
			lbuf[i][j] = zbuf[i*nlines + j];	// fill with the line
			obuf[i][j] = hval[i*nlines + j];
		}
	}

	/*
	 * if horizon cosines to be computed, need to allocate storage vector
	 */
	//	printf("here\n");
	if (parm.nbits > 1) {
		/* NOSTRICT */
		hcos = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
		//		fpixel_t *hcos[nlines];
		//		for(i = 0; i < nlines; i++) {
		//			hcos[i] = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
		//		}
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
	 * main loop, read in full line at a time
	 */
	fspace = parm.spacing;
	for (i = 0; i < nlines; i++) {

		/*
		 * find points that form horizons
		 */
		(void) (*horfun) (nsamps, lbuf[i], hbuf);

		/*
		 * if not mask output, compute and write horizons along each row
		 */
		if (parm.nbits > 1) {
			horval(nsamps, lbuf[i], fspace, hbuf, obuf[i]);
		}

		/*
		 * if mask output, set mask and write
		 */
		else {
			hormask(nsamps, lbuf[i], fspace, hbuf, thresh, obuf[i]);
		}


	}

	// bring back the results into hval
	for(i = 0; i < nlines; i++) {
		for(j = 0; j < nsamps; j++) {
			hval[i*nlines + j] = obuf[i][j];
		}
	}


	//	SAFE_FREE(zbuf);
	//	SAFE_FREE(hbuf);
	//	if (parm.nbits > 1) {
	//		//		SAFE_FREE(hcos);
	//		return hcos;
	//	} else {
	//		//		SAFE_FREE(hmask);
	//		return hmask;
	//	}

}
