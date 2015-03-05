#include "ipw.h"
#include "image.h"

int
access(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	if (image->flags & PIXEL_ACCESS) {
		return PIXEL_ACCESS;
	} else if (image->flags & FPIXEL_ACCESS) {
		return FPIXEL_ACCESS;
	} else {
		error("image '%s' has no access flags set", image_name(image));
	}
}
