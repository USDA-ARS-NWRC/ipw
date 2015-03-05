#include "ipw.h"
#include "image.h"

bool_t
write_fpbuf(
	IMAGE_T	     *image,		/* output image			  */
	FPIXEL       *buffer,		/* buffer of pixels to write	  */
	int           nsamples)		/* # of samples in buffer	  */
{
	int	      n_to_write;	/* # of bytes to write		  */
	int	      n_written;	/* # of bytes just written	  */

	assert(image != NULL);
	assert(buffer != NULL);
	assert(nsamples > 0);
	assert(image->flags & IPW_WRITE);
	assert(image->flags & FPIXEL_ACCESS);

	/*
	 *  If this is first IO call, then write image's headers first.
	 */
        if (! (image->flags & IPW_DATA_IO)) {
		if (! _write_hdrs(image)) {
			return FALSE;
		}
	}

	image->flags |= IPW_DATA_IO;

	/*
	 *  Output the floating-point pixel vectors to image.
	 */
	if (fpvwrite(image->fd, buffer, nsamples) != nsamples) {
		ipw_errno = IPWERR_MESSAGE;
		return FALSE;
	}
	return TRUE;
}
