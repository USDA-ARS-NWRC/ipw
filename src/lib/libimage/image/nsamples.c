#include "ipw.h"
#include "image.h"

int
nsamples(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	return image->nsamples;
}
