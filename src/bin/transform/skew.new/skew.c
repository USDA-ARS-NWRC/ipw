#include <math.h>
#include <strings.h>

#include "../skew.new/pgm.h"
#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "pixio.h"
#include "skewh.h"


/*
 * skew -- image horizontal skew/deskew
 */

void skew (
		fpixel_t        *image,		/* input image pointer	 */
		int				nlines,		/* number of input lines */
		int				nsamps,		/* number of input samples */
		int				nbands, 	/* number of bands */
		bool_t          fwd,		/* ? forward : inverse skew	 */
		double          angle,		/* skew angle			 */
		int				nthreads,	/* number of threads */
		int				*o_nsamps,	/* ouput image nsamps */
		fpixel_t        **oimage)	/* output image pointer	 */
{
	int line;						/* current line #		 */
	bool_t negflag;					/* ? angle < 0			 */
	int N;							/* number of pixels */
	int n;							/* counter */
	double slope;					/* tan(angle)			 */
	int max_skew;					/* maximum skew (# samples)	 */
	int o_samp;					/* # samples / output line	 */
	int *offset;						/* read offset into image line		 */
	int pix;						/* pixel in line */

	N = nlines * nsamps;
	offset = (int *)ecalloc(N, sizeof(int));

	/* set threads */
	if (nthreads != 1) {
		omp_set_dynamic(0);     		// Explicitly disable dynamic teams
		omp_set_num_threads(nthreads); 	// Use N threads for all consecutive parallel regions
	}

	/*
	 * calculate output line length
	 */
	if (angle >= 0.0) {
		negflag = FALSE;
	}
	else {
		negflag = TRUE;
		angle = -angle;
	}

	/*
	 ** can't find DTR function
	 **
	 **	slope = tan(DTR(angle));
	 */
	slope = tan(angle * (M_PI / 180.0));
	max_skew = (nlines - 1) * slope + 0.5;

	o_samp = nsamps;
	if (fwd) {
		o_samp += max_skew;
	}
	else {
		o_samp -= max_skew;
		assert(o_samp > 0);
	}
	*oimage = ecalloc(nlines * o_samp * nbands, sizeof(fpixel_t));
	*o_nsamps = o_samp;


	// offset into output line
	for (line = 0; line < nlines; ++line) {
		offset[line] = (negflag ? line : nlines - line - 1) * nbands * slope + 0.5;
	}

	/*
	 * process pixels
	 */
#pragma omp parallel shared(N, offset, nsamps, o_samp, nbands, oimage, image, fwd) private(n, pix, line)
#pragma omp for
	for (n = 0; n < N; ++n) {

		// current line
		line = floor(n/nsamps);

		// offset of pixel into line
		pix = n % nsamps;

		if (fwd) {

			(*oimage)[line * o_samp * nbands + offset[line] + pix] = image[line * nsamps * nbands + pix];

		}
		else {

			(*oimage)[line * o_samp * nbands + pix] = image[line * nsamps * nbands + offset[line] + pix];
		}

	}


}
