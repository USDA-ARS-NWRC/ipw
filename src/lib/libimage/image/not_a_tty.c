#include "ipw.h"
#include "image.h"

void
not_a_tty(
	IMAGE_T	       *image)		/* image			*/
{
	assert(image != NULL);

	no_tty(image->fd);
}
