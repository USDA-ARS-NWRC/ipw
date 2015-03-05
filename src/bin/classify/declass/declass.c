/*
** NAME
** 	declass -- declassify image by given class information
** 
** SYNOPSIS
**	declass (fdi, fdo);
** 	int fdi, fdo;
** 
** DESCRIPTION
** 	declass reads the given IPW image file, and writes an image file
**	where each of the image pixels is de-classified - class identifiers
**	are replaced by class representative values.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "crh.h"
#include "pgm.h"

void
declass(
	int		fdi,		/* input image file descriptor	*/
	int		fdo)		/* output image file descriptor	*/
{
	int		nbands;		/* #bands in input image	*/
	int		nlines;		/* #lines in input image        */
	int		nsamps;		/* #samps in input image        */
	CRH_T	      **crhpp;		/* -> crhpp for input image	*/
	pixel_t	       *ibuf;		/* input line buffer		*/
	pixel_t	       *ibufp;		/* -> in input line buffer	*/
	fpixel_t       *obuf;		/* output line buffer		*/
	fpixel_t       *obufp;		/* -> in input line buffer	*/
	int		line;		/* line loop counter		*/
	int		band;		/* band loopw counter		*/
	int		samp;		/* sample loop counter		*/


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);
	nbands = hnbands (fdi);

	crhpp = crh (fdi);


   /* allocate input buffer */

	ibuf = (pixel_t *) ecalloc (nsamps * nbands, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("Unable to allocate input line buffer");
	}

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * nbands, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("Unable to allocate output line buffer");
	}

   /* get classes sorted by class number */

	for (band = 0; band < nbands; band++) {
		if (crhsort (crhpp[band], SORT_BY_CLASS) == ERROR) {
			error ("error sorting classes");
		}
	}


   /* loop reading and converting each image line */

	for (line = 0; line < nlines; line++) {

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line+1);
		}

		ibufp = ibuf;
		obufp = obuf;

		for (samp = 0; samp < nsamps; samp++) {

			for (band = 0; band < nbands; band++) {

				if (*ibufp == 0) {
					*obufp++ = crh_lo (crhpp[band], 0);
				} else {
					assert (*ibufp > 0 &&
					     *ibufp <= crh_nclass(crhpp[band]));
					*obufp++ =
					      crh_rep (crhpp[band], *ibufp-1);
				}
				ibufp++;
			}
		}

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("error writing output image, line %d", line+1);
		}
	}
}
