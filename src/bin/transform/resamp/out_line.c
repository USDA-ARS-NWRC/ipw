/*
** NAME
** 	out_line -- compute means of output buffer and write to output file
** 
** SYNOPSIS
**	void
**	out_line (fdo, obuf, sums, npix, o_nsamps, nbands, ztype)
** 	int fdo;
**	fpixel_t *obuf;
**	double *sums;
**	int *npix;
**	int o_nsamps;
**	int nbands;
**	int ztype;
** 
** DESCRIPTION
**	out_line computes the means in the given buffer of sums and pixels
**	counts (if ztype = MEAN_VAL).  The resulting buffer of means, mins or
**	maxs is then written to the output file.
**
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
out_line (
	int		fdo,		/* output image file descriptor	 */
	fpixel_t       *obuf,		/* output buffer		 */
	double	       *sums,		/* sums for mean calculation	 */
	int	       *npix,		/* # pixels in mean for out pix	 */
	int		o_nsamps,	/* # samples in output image	 */
	int		nbands,		/* # samples in input image	 */
	int		ztype)		/* zoom type: MIN,MAX,or MEAN	 */
{
	int		samp;		/* sample loop counter		 */
	int		band;		/* band loop counter		 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	double	       *sumsp;		/* -> in sums buffer		 */
	int	       *npixp;		/* -> in npix buffer		 */


   /* Compute means if necessary */

	if (ztype == MEAN_VAL) {

		obufp = obuf;
		sumsp = sums;
		npixp = npix;
		
		for (samp = 0; samp < o_nsamps; samp++) {

			if (*npixp > 0) {
				for (band = 0; band < nbands; band++) {
					*obufp++ = *sumsp++ / *npixp;
				}
			} else {
				obufp += nbands;
				sumsp += nbands;
			}
			npixp++;
		}
	}

   /* Write output buffer to file */

	if (fpvwrite (fdo, obuf, o_nsamps) != o_nsamps) {
		error ("error writing output file");
	}
}
