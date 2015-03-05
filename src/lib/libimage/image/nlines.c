#include "ipw.h"
#include "image.h"

int
nlines(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	return image->nlines;
}
