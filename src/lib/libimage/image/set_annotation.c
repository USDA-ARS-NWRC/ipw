#include "ipw.h"
#include "image.h"

void
set_annotation(
	IMAGE_T	       *image,		/* output image	     */
	int		band,		/* band #	     */
	STRVEC_T       *annotation)	/* band's annotation */
{
	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));
	assert(bih_annot(image->bih[band]) == NULL);

	bih_annot(image->bih[band]) = annotation;
}
