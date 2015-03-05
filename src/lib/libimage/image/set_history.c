#include "ipw.h"
#include "image.h"

void
set_history(
	IMAGE_T	       *image,		/* output image	  */
	int		band,		/* band #	  */
	STRVEC_T       *history)	/* band's history */
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));
	assert(bih_history(image->bih[band]) == NULL);

	bih_history(image->bih[band]) = history;
}
