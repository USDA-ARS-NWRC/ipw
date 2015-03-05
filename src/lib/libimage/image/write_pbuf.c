#include "ipw.h"
#include "image.h"

bool_t
write_pbuf(
	IMAGE_T	     *image,		/* output image			  */
	PIXEL        *buffer,		/* buffer of pixels to write 	  */
	int	      nsamples)		/* # of samples in buffer	  */
{
	assert(image != NULL);
	assert(buffer != NULL);
	assert(nsamples > 0);
	assert(image->flags & IPW_WRITE);
	assert(image->flags & PIXEL_ACCESS);

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
	 *  Write the buffer to the image's file.
	 */
	if (pvwrite(image->fd, buffer, nsamples) != nsamples) {
		ipw_errno = IPWERR_MESSAGE;
		return FALSE;
	}
	return TRUE;
}
