/*
** NAME
**      imgsum -- sum an image
** 
** SYNOPSIS
**      imgsum (fdi, fdm)
**      int fdi, fdm;
** 
** DESCRIPTION
**      imgsum read the input image, sums all unmasked pixels and prints
**      the result to stdout.
** 
**
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
imgsum (
	int		fdi,		/* input image file desc	 */
	int		fdm)		/* mask image file desc		 */
{
	int		nsamps;		/* # samps in input image	 */
	int		nlines;		/* # lines in input image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	fpixel_t       *ibuf;		/* input image line buffer	 */
	fpixel_t       *ibufp;		/* -> in input image line 	 */
	pixel_t	       *mbuf;		/* mask image line buffer	 */
	pixel_t	       *mbufp;		/* -> in mask image line buffer	 */
	double		sum;		/* pixel sum			 */


	nsamps = hnsamps (fdi);
	nlines = hnlines (fdi);


   /* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (nsamps * nlines, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps * nlines, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask buffer");
		}
	} else {
		mbuf = (pixel_t *) NULL;
	}

   /* loop on image lines */

	sum = 0.0;

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image line %d", line);
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image line %d", line);
			}
		}

		ibufp = ibuf;
		mbufp = mbuf;

		/* loop on samples */

		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp) {
				sum += *ibufp;
			}

			ibufp++;
			mbufp++;
		}

	}

	SAFE_FREE(ibuf);
	SAFE_FREE(mbuf);

   /* print result */

	printf ("%f\n", sum);

}
