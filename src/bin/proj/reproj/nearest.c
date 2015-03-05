/*
** NAME
** 	nearest -- return value of nearest neighbor to given line, sample
** 
** SYNOPSIS
**	int
**	nearest (ibuf, line, samp, nlines, nsamps)
**	fpixel_t **ibuf;
**	float line, samp;
**	int nlines, nsamps;
** 
** DESCRIPTION
**	nearest returns the value in the buffer of the closest pixel to the
**	given line, sample indices (in decimal).  If the given line or sample
**	is outside the bounds of the image, 0 is returned.
** 
** RETURN VALUE
**	value of nearest neighbor
** 
*/

#include "ipw.h"
#include "pgm.h"
#include "gctp.h"

int
nearest(
	pixel_t       **ibuf,		/* input buffer			 */
	float		line,		/* line index (decimal)		 */
	float		samp,		/* sample index (decimal)	 */
	int		nlines,		/* # lines in buffer		 */
	int		nsamps)		/* # samples in buffer		 */
{
	int		iline;		/* line index of nearest pixel	 */
	int		isamp;		/* sample index of nearest pixel */
	int		value;		/* value of nearest neighbor	 */


   /* check for out of bounds */

	if (line < 0 || samp < 0 || line > (nlines - 1) || samp > (nsamps - 1))
		return (0);

   /* round indices to rearest integer */

	iline = line + 0.5;
	isamp = samp + 0.5;

   /* return value */

	value = ibuf[iline][isamp];
	if (value == NODATA)
		value = 0;
	return (ibuf[iline][isamp]);
}
