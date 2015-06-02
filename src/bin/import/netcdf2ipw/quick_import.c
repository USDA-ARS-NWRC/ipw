/*
 ** NAME
 ** 	quick_import -- import ASCII file to IPW image file in core
 **
 ** SYNOPSIS
 **	void quick_import (ifp, fdo, nlines, nsamps, nbands);
 **	FILE *ifp;
 **	int fdo;
 **	int nlines, nsamps, nbands;
 **
 ** DESCRIPTION
 ** 	Import reads the given ASCII data file and writes the output IPW
 **	image file.
 **
 */

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
quick_import (
		FILE	       *ifp,		/* file ptr to input file	 */
		int		fdo,		/* output image file descriptor	 */
		int		nlines,		/* # lines in output image	 */
		int		nsamps,		/* # samples in output image	 */
		int		nbands)		/* # bands in output image	 */
{
	int		nread;		/* # values read in fscanf()	 */
	int		npixels;	/* # of pixels			 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	bool_t		first_pixel;	/* flag for first pixel read	 */
	float		dummy;		/* dummy var for EOF test	 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	float	        min;		/* min value 			 */
	float	        max;		/* max value 			 */
	float           nmin[1];	/* min value array for newlqh    */
	float           nmax[1];	/* max value array for newlqh    */
	float	       *flar;		/* holding for scanned values	 */
	float          *flar_p;         /* pointer into flar		 */


	if (nbands != 1) {
		error ("Quick import can only work with one band");
	}
	npixels = nlines * nsamps;

	/* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (npixels, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	/* allocate input buffer -- ffscanf reads in float, not fpixel_t */

	flar = (float *) ecalloc (nsamps, sizeof(float));
	if (flar == NULL) {
		error ("can't allocate flar array");
	}


	min = 0;
	max = 0;
	first_pixel = TRUE;

	/* loop reading and writing complete lines of data */

	obufp = obuf;
	for (line = 0; line < nlines; line++) {
		/* quickly read in nsamps floating point values */
		if ((nread = ffscanf(ifp, nsamps, flar)) != nsamps) {
			if (nread == EOF) {
				error ("read past EOF, line %d", line);
			} else {
				error ("error reading input file, line %d", line);
			}
		}
		if (first_pixel) {
			first_pixel = FALSE;
			min = *flar;
			max = *flar;
		}
		flar_p = flar;
		for (samp = 0; samp < nsamps; samp++) {
			/* find min and max, and move to obuf */
			*obufp = *flar_p++;
			/* keep min and max of data */
			if (min > *obufp)  min = *obufp;
			if (max < *obufp)  max = *obufp;
			obufp++;
		}
	}

	free(flar);    /* done with this array */


	/* attempt another read to test for EOF */

	if ((nread = fscanf (ifp, "%f", &dummy)) == 1) {
		warn ("input file contains more than %d values - remainder ignored",
				nlines * nsamps * nbands);
	}

	/* create/write LQH quantized with mins/maxs */

	nmin[0] = min;
	nmax[0] = max;
	newlqh (fdo, nbands, nmin, nmax);

	/* write output image */

	obufp = obuf;
	for (line = 0; line < nlines; line++) {
		if (fpvwrite (fdo, obufp, nsamps) != nsamps) {
			error ("write error -- output file likely imcomplete");
		}
		obufp += nsamps;
	}

	free(obuf);

}
