/*
 ** NAME
 **      idewpt - convert vapor pressure to dew point temperature
 **
 ** SYNOPSIS
 **	void idewpt (fdi, fdm, fdo)
 **	int fdi;
 **	int fdm;
 **	int fdo;
 **
 ** DESCRIPTION
 **      idewpt converts vapor pressure to dew point temp
 **
 ** HISTORY
 **	10/21/92: Written by D. Marks, USGS, EPA, ERL/C;
 **
 */

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "envphys.h"
#include "pgm.h"

void
idewpt (
		int	fdi,			/* input image file descriptor		*/
		int	fdm,			/* mask image file descriptor		*/
		int	fdo,			/* output image file descriptor		*/
		int nthreads,		/* number of threads */
		double tol)			/* dew_point tolerance threshold */
{
	float	dpt;			/* dew point temp			*/
	float	ea;				/* vapor pressure			*/
	float	dpt_min = 0.0;	/* min dpt in output image		*/
	float	dpt_max = 0.0;	/* max dpt in output image		*/

	fpixel_t *ibuf;			/* pointer to input buffer	*/
	fpixel_t *obuf;			/* pointer to output buffer	*/
	pixel_t	*mbuf;			/* pointer to mask buffer	*/
	int	nsamps;				/* # samples per line		*/
	int	nlines;				/* # lines in image		*/
	int	first_pixel;		/* flag for first pixel procced */
	int	nbytes;				/* #bytes to write		*/
	int N;					/* number of pixels */
	int n;					/* loop over pixels */

	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	N = nsamps * nlines;
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;

	/* set threads */
	if (nthreads != 1) {
		omp_set_dynamic(0);     		// Explicitly disable dynamic teams
		omp_set_num_threads(nthreads); 	// Use N threads for all consecutive parallel regions
	}

	/* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (N * IBANDS, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

	/* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (N, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate input buffer");
		}
	} else {
		mbuf = NULL;
	}

	/* Allocate output buffer */

	obuf = (fpixel_t *) ecalloc (N * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	/* read input image */
	if (fpvread (fdi, ibuf, N) != N) {
		error ("input image read error");
	}

	/* read mask image */
	if (fdm != ERROR) {
		if (pvread (fdm, mbuf, N) != N) {
			error ("mask image read error");
		}
	}

	/* read input data and do calculations */

	if (nthreads != 1) {
#pragma omp parallel shared(ibuf, mbuf, obuf, fdm, tol) private(n, ea, dpt)
		{
#pragma omp for
			for (n = 0; n < N; n++) {

				/* if not a masked point */
				if (fdm == ERROR || mbuf[n]) {

					/* extract input band */
					ea = ibuf[n];

					/*	calculate dew point temp	*/

					dpt = (float) dew_pointp((double) ea, tol);

					/*	convert from K to C	*/

					dpt -= FREEZE;

					/* set output band */

					obuf[n] = dpt;

				} else {
					obuf[n] = 0.0;
				}

			}
		}
	} else {
		for (n = 0; n < N; n++) {

			/* if not a masked point */
			if (fdm == ERROR || mbuf[n]) {

				/* extract input band */
				ea = ibuf[n];

				/*	calculate dew point temp	*/

				dpt = (float) dew_pointp((double) ea, tol);

				/*	convert from K to C	*/

				dpt -= FREEZE;

				/* set output band */

				obuf[n] = dpt;

			} else {
				obuf[n] = 0.0;
			}

		}
	}

	/* get the min/max */
	first_pixel = TRUE;
	for (n = 0; n < N; n++) {

		// loop through each pixel that isn't masked
		if (fdm == ERROR || mbuf[n]) {

			// if it's the first pixel
			if (first_pixel) {
				dpt_min = obuf[n];
				dpt_max = obuf[n];
				first_pixel = FALSE;
			} else {
				dpt_min = MIN(obuf[n], dpt_min);
				dpt_max = MAX(obuf[n], dpt_max);
			}
		}
	}

	/* create/write LQH for output image */

	newlqh (fdo, dpt_min, dpt_max);

	/* output to file */
	if (fpvwrite (fdo, obuf, N) != N) {
		error ("write error output image");
	}

	(void) fpclose(fdo);


}
