/*
** NAME
** 	iobuf -- allocate I/O buffers for ghorizon
** 
** SYNOPSIS
**	void iobuf (nlines, nsamps, s_nbands, got_solar, fdm,
**		    ibuf, sbuf, mbuf, obuf)
**	int nlines, nsamps, s_nbands;
**	bool_t got_solar;
**	int fdm;
**	fpixel_t **ibuf;
**	pixel_t **sbuf, **mbuf;
**	fpixel_t **obuf;
** 
** DESCRIPTION
**	iobuf allocates the buffer for the entire input elevation file, the
**	input line buffers for the solar and mask files and the output line
**	buffer for the output file.
** 
*/

#include "ipw.h"
#include "pgm.h"

void
iobuf(
	int		nlines,		/* # lines in input image	 */
	int		nsamps,		/* # samples in input image	 */
	int		s_nbands,	/* # bands in solar image	 */
	bool_t		got_solar,	/* flag for solar image given	 */
	int		fdm,		/* mask image file descriptor	 */
	fpixel_t     ***ibuf,		/* returned -> to input buffer	 */
	pixel_t	      **sbuf,		/* returned -> to solar buffer	 */
	pixel_t	      **mbuf,		/* returned -> to mask buffer	 */
	fpixel_t      **obuf)		/* returned -> to output buffer	 */
{

   /* allocate input buffer - need to read in the entire file */

	*ibuf = (fpixel_t **) allocnd (sizeof(fpixel_t), 2, nlines, nsamps);
	if (*ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* allocate solar buffer */

	if (got_solar) {
		*sbuf = (pixel_t *) ecalloc (s_nbands * nsamps, sizeof(pixel_t));
		if (*sbuf == NULL) {
			error ("can't allocate solar image buffer");
		}
	}

   /* allocate mask buffer */

	if (fdm != ERROR) {
		*mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (*mbuf == NULL) {
			error ("can't allocate mask image buffer");
		}
	}

   /* allocate output buffer */

	*obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (*obuf == NULL) {
		error ("can't allocate output buffer");
	}
}
