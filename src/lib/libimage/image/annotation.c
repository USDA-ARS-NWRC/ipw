#include "ipw.h"
#include "image.h"

STRVEC_T *
annotation(
	IMAGE_T	       *image,		/* image	*/
	int		band)		/* band #	*/
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));

	return bih_annot(image->bih[band]);
}
