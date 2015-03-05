/*
** NAME
** 	hist - compute image histogram
** 
** SYNOPSIS
**	#include "classeq.h"
**
**	hist_t *hist (i_fd, m_fd, h_nsamps)
** 	int i_fd, mf_fd;
**	int h_nsamps;
** 
** DESCRIPTION
** 	hist reads the give IPW image and mask image files and
**	computes the histogram.  The histogram returned is an array
**	where the offset corresponds to the raw pixel value and the
**	value is the count of pixels (within the given mask) with that
**	pixel value.
** 
** RESTRICTIONS
**
**	Assumes headers have already been read.
**	Assumes single-band input image.
** 
** RETURN VALUE
**
**	Pointer to histogram array.
*/

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "pixio.h"

#include "pgm.h"

hist_t
*hist(
	int             i_fd,		/* input file descriptor	 */
	int             m_fd,		/* mask file descriptor		 */
	int	        h_nsamps)	/* # samps in histogram          */
{
	hist_t         *histo;		/* output histogram            	 */
	int             i_nlines;	/* # input lines		 */
	int             i_nsamps;	/* # samples / input line	 */
	int             line;		/* image line counter		 */
	pixel_t        *i_buf;		/* -> image input buffer	 */
	pixel_t        *m_buf;		/* -> mask input buffer		 */


	i_nlines = hnlines (i_fd);
	i_nsamps = hnlines (i_fd);


   /* allocate histogram array */

	histo = (hist_t *) ecalloc (h_nsamps, sizeof(hist_t));
	if (histo == NULL) {
		error("can't allocate histogram array");
	}

   /* allocate I/O arrays */

	i_buf = (pixel_t *) ecalloc (i_nsamps, sizeof(pixel_t));
	if (i_buf == NULL) {
		error("can't allocate image input buffer");
	}

	if (m_fd == ERROR) {
		m_buf = NULL;
	} else {
		m_buf = (pixel_t *) ecalloc(i_nsamps, sizeof(pixel_t));
		if (m_buf == NULL) {
			error("can't allocate mask input buffer");
		}
	}

   /* read image and accumulate histogram */

	for (line = 0; line < i_nlines; ++line) {
		if (pvread(i_fd, i_buf, i_nsamps) != i_nsamps) {
			error("image read failed, line %d", line);
		}

		if (m_fd != ERROR) {
			if (pvread(m_fd, m_buf, i_nsamps) != i_nsamps) {
				error("mask read failed, line %d", line);
			}
		}

		histx (i_buf, m_buf, i_nsamps, histo);
	}

	free (i_buf);
	free (m_buf);

	return (histo);
}
