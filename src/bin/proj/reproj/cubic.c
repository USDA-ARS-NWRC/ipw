/*
** NAME
** 	cubic -- use cubic convolution interpolation for given line, sample
** 
** SYNOPSIS
**	int
**	cubic (ibuf, line, samp, nlines, nsamps)
**	pixel_t **ibuf;
**	float line, samp;
**	int nlines, nsamps;
** 
** DESCRIPTION
**	cubic returns the value in the buffer that is the result of
**	cubic convolution interpolation for the given line, sample indices
**	(in decimal).  	If the given line or sample is outside the bounds
**	of the image, or if any of the 16 surrounding points to be used in
**	the interpolation is NODATA, 0 is returned.
**
**	The image data is "replicated" one pixel in each direction for the
**	purposes of convolution.
** 
** RETURN VALUE
**	result of cubic convolution interpolation
** 
** WARNINGS
**	Cubic convolution is slower than bilinear interpolation.  A program
**	equally optimized would be about half as fast.  This program is not
**	optimized and will be a LOT slower.
** 
** APPLICATION USAGE
**	Cubic convolution generally gives better results than bilinear
**	interpolation at the expense of speed.  Bilinear smooths the data
**	more than Cubic convolution.
**
** HISTORY
**	8/20/93	 Written by Dana Jacobsen, ERL-C.
**
** SEE ALSO
**	J. G. Moik, Digital Processing of Remotely Sensed Images, NASA
**	Technical Report SP-431, Section 3.3.2 "Resampling".
**
*/

#include "ipw.h"
#include "pgm.h"
#include "gctp.h"

#define DEBUG 0

int
cubic(
	pixel_t       **ibuf,		/* input buffer			 */
	float		line,		/* line index (decimal)		 */
	float		samp,		/* sample index (decimal)	 */
	int		nlines,		/* # lines in buffer		 */
	int		nsamps)		/* # samples in buffer		 */
{
	int		iline;		/* integer line index for interp */
	int		isamp;		/* integer line index for interp */
	int		ilinem1;	/* iline -1			 */
	int		isampm1;	/* isamp -1			 */
	int		ilinep2;	/* iline +2			 */
	int		isampp2;	/* isamp +2			 */
	double		alpha;		/* intermediate slope		 */
	double		beta;		/* intermediate slope		 */
	double		value;		/* result of interpolation	 */
	double		a1, a2, a3, a4; /* intermediate values		 */
	double		b1, b2, b3, b4; /* intermediate values		 */
#if DEBUG
	double		bilin;
#endif


	iline = line;
	isamp = samp;

#if DEBUG
	fprintf(stderr, "cubic: (%d,%d) (%f, %f)", iline, isamp, line, samp);
#endif
   /* check for out of bounds of image */

	if (iline < 0  ||  isamp < 0  ||
	    (iline+1) >= nlines  ||  (isamp+1) >= nsamps  ) {
#if DEBUG
		fprintf(stderr, " Out of Bounds -- return 0\n");
#endif
		return (0);
	}

	if ( (iline+2) >= nlines )
		ilinep2 = iline+1;
	else
		ilinep2 = iline+2;

	if ( (isamp+2) >= nsamps )
		isampp2 = isamp+1;
	else
		isampp2 = isamp+2;

	if ( iline < 1 )
		ilinem1 = iline;
	else
		ilinem1 = iline-1;

	if ( isamp < 1 )
		isampm1 = isamp;
	else
		isampm1 = isamp-1;

   /* check the 16 surrounding points for no data values */

	if (ibuf[ilinem1][isampm1] == NODATA ||
            ibuf[ilinem1][isamp]   == NODATA ||
	    ibuf[ilinem1][isamp+1] == NODATA ||
	    ibuf[ilinem1][isampp2] == NODATA ||
	    ibuf[iline][isampm1] == NODATA ||
            ibuf[iline][isamp]   == NODATA ||
	    ibuf[iline][isamp+1] == NODATA ||
	    ibuf[iline][isampp2] == NODATA ||
	    ibuf[iline+1][isampm1] == NODATA ||
            ibuf[iline+1][isamp]   == NODATA ||
	    ibuf[iline+1][isamp+1] == NODATA ||
	    ibuf[iline+1][isampp2] == NODATA ||
	    ibuf[ilinep2][isampm1] == NODATA ||
            ibuf[ilinep2][isamp]   == NODATA ||
	    ibuf[ilinep2][isamp+1] == NODATA ||
	    ibuf[ilinep2][isampp2] == NODATA    ) {
#if DEBUG
		fprintf(stderr, " No Data -- return 0\n");
#endif
		return (0);
	}

	alpha =  line - (double) iline;
	beta = samp - (double) isamp;

	a1 = alpha * ( (1.0 - alpha) * (1.0 - alpha) );
	a2 = 1.0 - ( 2.0 * alpha * alpha) + (alpha * alpha * alpha);
	a3 = alpha * (1.0 + alpha - (alpha * alpha) );
	a4 = (alpha * alpha) * (1.0 - alpha);

	b1 = beta * ( (1.0 - beta) * (1.0 - beta) );
	b2 = 1.0 - ( 2.0 * beta * beta) + (beta * beta * beta);
	b3 = beta * (1.0 + beta - (beta * beta) );
	b4 = (beta * beta) * (beta - 1.0);

#define Ix(smp)	(   -a1 * ibuf[ilinem1][(smp)]  \
                  +  a2 * ibuf[iline][(smp)]    \
                  +  a3 * ibuf[iline+1][(smp)]  \
                  -  a4 * ibuf[ilinep2][(smp)]   )

	value =  -b1 * Ix(isampm1) + b2 * Ix(isamp)
                + b3 * Ix(isamp+1) + b4 * Ix(isampp2);

	if (value < 0)
		value = 0;

#if DEBUG
        bilin = (1 - alpha - beta + (alpha*beta)) * ibuf[iline][isamp] +
                 (beta - (alpha*beta)) * ibuf[iline][isamp+1] +
                 (alpha - (alpha*beta)) * ibuf[iline+1][isamp] +
                 (alpha*beta) * ibuf[iline+1][isamp+1];

	fprintf(stderr, " NN:%d, BI:%d, CC:%d\n",
	 ibuf[(int)(line+0.5)][(int)(samp+0.5)], (int)bilin, (int)value);
#endif

	return ((int)value);
}
