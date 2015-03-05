/*
** NAME
** 	ipw2bin -- read input image and write to output file
** 
** DESCRIPTION
**	ipw2bin read the input image and writes the floating point data to
**	the output file.
** 
*/

#include "pgm.h"

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"


void
ipw2bin(
	int		fdi,		/* input image file descriptor	 */
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output file descriptor	 */
	float		nodata)		/* value for masked points	 */
{
	int		nlines;		/* #lines in input image	 */
	int		nsamps;		/* #samps in input image	 */
	int		nbytes;		/* # bytes in buffer		 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	fpixel_t       *ibuf;		/* input line buffer		 */
	fpixel_t       *ibufp;		/* -> in input line buffer	 */
	pixel_t        *mbuf;		/* mask line buffer		 */
	pixel_t        *mbufp;		/* -> in mask line buffer	 */


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);

   /* allocate input line buffer */

	ibuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("error allocating input buffer");
	}
	nbytes = nsamps * sizeof(fpixel_t);

   /* allocate mask line buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("error allocating mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* loop on lines reading and writing data */

	for (line = 0; line < nlines; line++) {

		/* read input line */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line);
		}

		/* apply mask if provided */

		if (fdm != ERROR) {

			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image, line %d", line);
			}

			ibufp = ibuf;
			mbufp = mbuf;

			for (samp = 0; samp < nsamps; samp++) {
				if (*mbufp == 0)
					*ibufp = nodata;
				mbufp++;
				ibufp++;
			}
		}

		/* write line to output file */

		if (uwrite (fdo, (addr_t) ibuf, nbytes) != nbytes) {
			error ("error writing output file, line %d", line);
		}
	}

	SAFE_FREE(ibuf);
	SAFE_FREE(mbuf);
}
