#include "ipw.h"
#include "image.h"

GEOH_T *
get_geoh(
	IMAGE_T	       *image,		/* image	*/
	int		band)		/* band #	*/
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));

	if (image->geoh == NULL) {
		return NULL;
	} else {
		return image->geoh[band];
	}
}
