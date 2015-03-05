#include "ipw.h"
#include "image.h"

int
nbands(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	return image->nbands;
}
