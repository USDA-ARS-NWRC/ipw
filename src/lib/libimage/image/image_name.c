#include "ipw.h"
#include "image.h"

char *
image_name(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	if (STREQ(image->name, IPW_STDIN) && (image->flags & IPW_READ)) {
		return "(standard input)";
	}
	else if (STREQ(image->name, IPW_STDOUT) && (image->flags & IPW_WRITE)) {
		return "(standard output)";
	}
	else if (image->name != NULL) {
		return image->name;
	}
	else
		return "(no name)";
}
