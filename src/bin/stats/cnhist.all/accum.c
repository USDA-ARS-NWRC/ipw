/*
** NAME
** 	accum -- accumulate histogram with given image
** 
** SYNOPSIS
**	void accum (fdi, fdm, ibuf, mbuf, nlines, nsamps, hist, pixcount);
**	int fdi, fdm;
**	pixel_t *ibuf, *mbuf;
**	int nlines, nsamps;
**	int *hist;
**	int *pixcount;
** 
** DESCRIPTION
** 	accum reads the given IPW image and adds the image data to the
**	cumulative histogram array.
** 
*/

#include "ipw.h"
#include "pixio.h"
#include "pgm.h"

void
accum (
	int		fdi,		/* input image file descriptor	 */
	int		fdm,		/* mask image file descriptor	 */
	pixel_t        *ibuf,		/* input line buffer		 */
	pixel_t        *mbuf,		/* mask buffer			 */
	int		nlines,		/* #lines in input image	 */
	int		nsamps,		/* #samples in input image	 */
	int	       *hist,		/* histogram array		 */
	int	       *pixcount)	/* total # pixels		 */
{
	int		count;		/* pixel count for this image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	pixel_t	       *ibufp;		/* -> in input line buffer	 */
	pixel_t	       *mbufp;		/* -> in mask line buffer	 */


   /* loop on lines and samples, accumulating histogram */

	count = 0;
	mbufp = mbuf;

	for (line = 0; line < nlines; line++) {

		/* read next image line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("Error reading input image, line %d\n", line);
		}

		ibufp = ibuf;

		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp) {
				hist[*ibufp]++;
				count++;
			}
			ibufp++;
			mbufp++;
		}
	}

	*pixcount += count;
}
