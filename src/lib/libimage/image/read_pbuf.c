#include "ipw.h"
#include "image.h"
#include "pixio.h"

bool_t
read_pbuf(
	IMAGE_T	    *image,	  /* input image			*/
	PIXEL       *buffer,	  /* line buffer to store pixels	*/
	int	     nsamples)	  /* # of samples requested		*/
{
	int	     n_to_read;	  /* # of pixel vectors to be read	*/
	int	     n_read;	  /* # of pixel vectors just read	*/

	assert(image != NULL);
	assert(buffer != NULL);
	assert(nsamples > 0);
	assert(image->flags & IPW_READ);
	assert(image->flags & PIXEL_ACCESS);

	image->flags |= IPW_DATA_IO;

	n_to_read = nsamples;
	do {
	 	n_read = pvread(image->fd, buffer, n_to_read);
		if (n_read <= 0)
			return FALSE;
		n_to_read -= n_read;
		buffer += (n_read * image->nbands);
	} while (n_to_read > 0);

	return TRUE;
}
