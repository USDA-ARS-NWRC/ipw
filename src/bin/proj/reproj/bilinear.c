/*
** NAME
** 	bilinear -- use bilinear interpolation for given line, sample
** 
** SYNOPSIS
**	int
**	bilinear (ibuf, line, samp, nlines, nsamps)
**	pixel_t **ibuf;
**	float line, samp;
**	int nlines, nsamps;
** 
** DESCRIPTION
**	bilinear returns the value in the buffer that is the result of
**	bilinear interpolation for the given line, sample indices (in decimal).
**	If the given line or sample is outside the bounds of the image,
**	or if any of the 4 surrounding points to be used in the interpolation
**	is NODATA, 0 is returned.
** 
** RETURN VALUE
**	result of bilinear interpolation
**
** HISTORY
**	10/14/94  Added 1-pixel replication at input image border.
**		  (as in the cc option).  Rusty Dodson, ERL-C. 
*/

#include "ipw.h"
#include "pgm.h"
#include "gctp.h"

int
bilinear(
	pixel_t       **ibuf,		/* input buffer			 */
	float		line,		/* line index (decimal)		 */
	float		samp,		/* sample index (decimal)	 */
	int		nlines,		/* # lines in buffer		 */
	int		nsamps)		/* # samples in buffer		 */
{
	int		line1;		/* lower line index for interp	 */
	int		line2;		/* upper line index for interp	 */
	int		samp1;		/* lower sample index for interp */
	int		samp2;		/* upper sample index for interp */
	float		alpha;		/* intermediate slope		 */
	float		beta;		/* intermediate slope		 */
	float		alpha_beta;	/* alpha * beta			 */
	float		value;		/* result of interpolation	 */


	line1 = line;
	samp1 = samp;

   /* check for out of bounds of image */

	/****if (line < 0 || line2 >= nlines || samp < 0 || samp2 >= nsamps)
		return (0);
	***/

	if (line < 0 || (line+1) > nlines || samp < 0 || (samp+1) >= nsamps )
		return (0);

	line2 = line1 + 1;
	samp2 = samp1 + 1;

	if ( (line2 < 1) || (line2 >= nlines) )
	   line2 = line1;
	if ( (samp2 < 1) || (samp2 >= nsamps) )
	   samp2 = samp1;
	
   /* check for no data values */

	if (ibuf[line1][samp1] == NODATA || ibuf[line1][samp2] == NODATA ||
	    ibuf[line2][samp1] == NODATA || ibuf[line2][samp2] == NODATA) {
		return (0);
	}

	alpha =  line - line1;
	beta = samp - samp1;
	alpha_beta = alpha * beta;

	value = (1 - alpha - beta + alpha_beta) * ibuf[line1][samp1] +
		 (beta - alpha_beta) * ibuf[line1][samp2] +
		 (alpha - alpha_beta) * ibuf[line2][samp1] +
		 alpha_beta * ibuf[line2][samp2];

	return ((int)value);
}
