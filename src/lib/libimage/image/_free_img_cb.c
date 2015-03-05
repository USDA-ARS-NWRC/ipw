/*
** NAME
**	_free_img_cb - free an image control-block
**
** SYNOPSIS
**	#include "_image.h"
**
**	IMAGE_T *
**	_free_img_cb(
**		IMAGE_T	    *icb)	|* -> image control block *|
**
** DESCRIPTION
**	This routine deallocates the memory associated with the
**	given image control-block.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES READ
**
** GLOBAL VARIABLES MODIFIED
*/

#include "ipw.h"
#include "image.h"
#include "_image.h"

void
_free_img_cb(
	IMAGE_T	    *icb)	/* -> image control block */
{
	int	     b;		/* band index		  */

	if (icb != NULL) {
		SAFE_FREE(icb->name);
		SAFE_FREE(icb->nbits);

/*
 *  Since the library function "uclose" handles the free'ing of the BI
 *  headers, we don't need to deal with:
 *
 *		icb->bih
 *
 *  Since the library function "fpclose" handles the free'ing of the LQ
 *  headers, we don't need to deal with:
 *
 *		icb->lqh
 */

		if (icb->geoh != NULL) {
			/*
			 *  If an output image, assume GEO headers are in
			 *  either reserved memory (i.e., the headers of
			 *  an input image), or static memory.  So only
			 *  free GEO headers if input image.
			 */
			if (icb->flags & IPW_READ) {
				for (b = 0; b < icb->nbands; b++) {
					SAFE_FREE(icb->geoh[b]);
				}
			}

			/*
			 *  For input and output images, free the array of
			 *  pointers to GEO headers.
			 */
			SAFE_FREE(icb->geoh);
		}

		SAFE_FREE(icb);
	}
}
