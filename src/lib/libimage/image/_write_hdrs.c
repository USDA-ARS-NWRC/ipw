/*
** NAME
**	_write_hdrs -- write out the headers for an output image
**
** SYNOPSIS
**	#include "_image.h"
**
**	bool_t
**	_write_hdrs(
**		IMAGE_T       *image)	|* image			*|
**
** DESCRIPTION
**	This routine writes out all the headers for an output image.
**
** RETURN VALUE
**	TRUE	All headers written.
**
**	FALSE	Error occurred.  "ipw_errno" is one of the following:
**
**		IPWERR_
*/

#include "ipw.h"
#include "image.h"
#include "_image.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "hdrio.h"	/* for boimage() */

bool_t
_write_hdrs(
	IMAGE_T        *image)		/* output image			*/
{
	assert(image != NULL);
	assert(image->flags & IPW_WRITE);

	/*
	 *  Output BI headers.
	 */
	if (bihwrite(image->fd, image->bih) == ERROR) {
		ipw_errno = IPWERR_WR_BIH;
		return FALSE;
	}

	/*
	 *  Make sure LQ headers were given if access is FPIXEL.
	 */
	if (image->flags & FPIXEL_ACCESS) {
		if (image->lqh == NULL) {
			bug("writing floating-pixel data without LQ headers");
		}
	}

	if (image->lqh != NULL) {
		if (lqhwrite(image->fd, image->lqh) == ERROR) {
			ipw_errno = IPWERR_WR_LQH;
			return FALSE;
		}
	}

	if (image->geoh != NULL) {
		if (geohwrite(image->fd, image->geoh) == ERROR) {
			ipw_errno = IPWERR_WR_GEOH;
			return FALSE;
		}
	}

	/*
	 *  Terminate image headers.
	 */
	if (boimage(image->fd) == ERROR) {
		ipw_errno = IPWERR_WR_BOI;
		return FALSE;
	}

	return TRUE;
}
