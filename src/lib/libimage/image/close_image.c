#include "ipw.h"
#include "image.h"
#include "_image.h"
#include "fpio.h"

bool_t
close_image(
	IMAGE_T       *image)	/* image			*/
{
	assert(image != NULL);

	if (image->flags & FPIXEL_ACCESS) {
		if (fpclose(image->fd) == ERROR) {
			ipw_errno = IPWERR_MESSAGE;
			return FALSE;
		}
	}

	else if (image->flags & PIXEL_ACCESS) {
		if (pxclose(image->fd) == ERROR) {
			ipw_errno = IPWERR_MESSAGE;
			return FALSE;
		}
	}

	else {
		bug("image has neither FPIXEL_ACCESS nor PIXEL_ACCESS");
	}

	_free_img_cb(image);
	return TRUE;
}
