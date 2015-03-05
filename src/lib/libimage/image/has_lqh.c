#include "ipw.h"
#include "image.h"

bool_t
has_lqh(
	IMAGE_T	       *image,		/* image	*/
	int		band)		/* band #	*/
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));

	if (image->lqh == NULL) {
		return FALSE;
	}
	return (image->lqh[band] == NULL);
}
