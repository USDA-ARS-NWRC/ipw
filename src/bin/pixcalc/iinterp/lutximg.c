/*
** NAME
** 	lutximg -- map image through lookup table
** 
** SYNOPSIS
**	lutximg (fdi, nlines, nsamps, lut, fdo)
** 	int fdi;
**	int nlines, nsamps;
**	fpixel_t *lut;
**	int fdo;
** 
** DESCRIPTION
** 	lutximg maps the given IPW image through the given lookup table
**	array and write the corresponding output image.
*/

#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
lutximg (
	int             fdi,		/* input image file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / image line	 */
	REG_3 fpixel_t  *lut,		/* -> LUT array		 	 */
	int             fdo)		/* output image file descriptor	 */
{
	pixel_t        *ibuf;		/* input image I/O buffer	 */
	pixel_t        *ibufp;		/* -> in input image I/O buffer	 */
	fpixel_t       *obuf;		/* output image I/O buffer	 */
	fpixel_t       *obufp;		/* -> in output image I/O buffer */
	int             line;		/* line loop counter		 */
	int             samp;		/* sample loop counter		 */


   /* allocate I/O buffers */

	ibuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("can't allocate image input buffer");
	}

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate image output buffer");
	}

   /* I/O loop */

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("image read failed, line %d", line);
		}

		ibufp = ibuf;
		obufp = obuf;

		/* loop through samples */
		/* map pixel value through lookup table */

		for (samp = 0; samp < nsamps; samp++) {

			*obufp++ = lut[*ibufp++];
		}

		/* write line to output image */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("image write failed, line %d", line);
		}
	}
}
