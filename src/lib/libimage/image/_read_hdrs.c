/*
** NAME
**	_read_hdrs - read and store the headers for an image
**
** SYNOPSIS
**	#include "_image.h"
**
**	bool_t
**	_read_hdrs(
**		IMAGE_T	   *image)	|* input image			*|
**
** DESCRIPTION
**	This routine reads in the headers for an input image.  All the
**	headers are stored in the image's control-block.
**
**	Note:  currently the only header types loaded by this routine are:
**
**		BI	basic image headers
**		LQ	linear quantization headers
**		GEO	geodetic headers
**
**	All the other types of headers are ignored.
**	Eventually, we should add all the other known headers types.
**
** RETURN VALUE
**	TRUE	The headers were read.
**
**	FALSE	An error occurred.  "ipw_errno" is set to one of these:
**
**		IPWERR_RD_BIH	error reading image's basic image headers
**
** GLOBAL VARIABLES MODIFIED
**	ipw_errno
*/

#include "ipw.h"
#include "image.h"
#include "_image.h"
#include "gethdrs.h"

bool_t
_read_hdrs(
	IMAGE_T	       *image)		/* input image			*/
{
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	GETHDR_T *request[] = {&h_lqh, &h_geoh, 0};

	int nbands;
	int i;

	assert(image != NULL);
	assert(image->flags & IPW_READ);

	/*
	 *  Read the BIH for the image
	 */
	if ((image->bih = bihread(image->fd)) == NULL) {
		ipw_errno = IPWERR_RD_BIH;
		return FALSE;
	}

	/*
	 * Ingest LQ and GEO headers; skip other headers
	 */
	gethdrs(image->fd, request, NO_COPY, ERROR);

	image->lqh = (LQH_T **) hdr_addr(h_lqh);
	hdr_addr(h_lqh) = NULL;

	image->geoh = (GEOH_T **) hdr_addr(h_geoh);
	hdr_addr(h_geoh) = NULL;

	return TRUE;
}
