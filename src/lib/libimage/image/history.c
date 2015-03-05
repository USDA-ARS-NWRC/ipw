#include "ipw.h"
#include "image.h"

STRVEC_T *
history(
	IMAGE_T	       *image,		/* image	*/
	int		band)		/* band #	*/
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));

	return bih_history(image->bih[band]);
}
