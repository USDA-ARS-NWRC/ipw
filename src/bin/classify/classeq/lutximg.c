/*
** NAME
** 	lutximg -- map image through lookup table
** 
** SYNOPSIS
**	lutximg (i_fd, m_fd, nlines, nsamps, lut, o_fd)
** 	int i_fd, m_fd;
**	int nlines, nsamps;
**	pixel_t *lut;
**	int o_fd;
** 
** DESCRIPTION
** 	lutximg maps the given IPW image through the given lookup table
**	array and write the corresponding output image.
*/

#include "ipw.h"
#include "pixio.h"

#include "pgm.h"

void
lutximg (
	int             i_fd,		/* input image file descriptor	 */
	int             m_fd,		/* mask image file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / image line	 */
	REG_3 pixel_t  *lut,		/* -> LUT array		 	 */
	int             o_fd)		/* output image file descriptor	 */
{
	pixel_t        *buf;		/* -> input image I/O buffer	 */
	pixel_t        *mbuf =NULL;	/* -> mask image I/O buffer	 */
	int             line;		/* current line #		 */


   /* allocate I/O buffers */

	buf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (buf == NULL) {
		error ("can't allocate image I/O buffer");
	}

	if (m_fd != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask I/O buffer");
		}
	}

   /* I/O loop */

	for (line = 0; line < nlines; ++line) {
		REG_1 pixel_t  *bufp;		/* -> current pixel	    */
		REG_2 pixel_t  *mbufp =NULL;	/* -> current pixel in mask */
		REG_5 int       samp;		/* loop counter		    */


   /* read line */

		if (pvread(i_fd, buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}

		if (m_fd != ERROR) {
			if (pvread(m_fd, mbuf, nsamps) != nsamps) {
				error("mask read failed, line %d", line);
			}
			mbufp = mbuf;
		}

		bufp = buf;

   /* loop through samples (sample # unimportant) */

		samp = nsamps;

   /* map pixel value through lookup table */

		do {
			if (m_fd == ERROR) {
				*bufp = lut[*bufp];
			} else if (*mbufp != 0) {
				*bufp = lut[*bufp];
			} else {
				*bufp = NO_DATA;
			}
			bufp++;
			mbufp++;
		} while (--samp > 0);

   /* write line */

		if (pvwrite(o_fd, buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}
}
