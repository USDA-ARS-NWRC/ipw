#include "ipw.h"
#include "image.h"

FPIXEL
lqh_max(
	IMAGE_T	       *image,		/* image	*/
	int		band)		/* band #	*/
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));
	assert(has_lqh(image, band));

	return image->lqh_maxs[band];
}
