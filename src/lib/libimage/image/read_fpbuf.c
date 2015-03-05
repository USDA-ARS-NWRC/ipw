#include "ipw.h"
#include "image.h"
#include "fpio.h"

bool_t
read_fpbuf(
	IMAGE_T	     *image,		/* input image			*/
	FPIXEL       *buffer,		/* line buffer to store pixels	*/
	int	      nsamples)		/* # of samples requested	*/
{
	int	      n_to_read;	/* # of samples to be read	*/
	int	      n_read;		/* # of samples just read	*/

	assert(image != NULL);
	assert(buffer != NULL);
	assert(nsamples > 0);
	assert(image->flags & IPW_READ);
	assert(image->flags & FPIXEL_ACCESS);

	image->flags |= IPW_DATA_IO;

	n_to_read = nsamples;
	do {
	 	n_read = fpvread(image->fd, buffer, n_to_read);
		if (n_read <= 0)
			return FALSE;
		n_to_read -= n_read;
		buffer += (n_read * image->nbands);
	} while (n_to_read > 0);

	return TRUE;
}
