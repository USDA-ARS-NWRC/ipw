/*
** NAME
** 	headers  -- read input headers for reproj_area
** 
** SYNOPSIS
**	void
**	headers (fdi, bline, bsamp, dline, dsamp)
**	int fdi;
**	double *bline, *bsamp, *dline, *dsamp;
** 
** DESCRIPTION
**	Headers reads/ingests/checks the geodetic header of the given
**	mask image for program reproj_area
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "geoh.h"
#include "pgm.h"

void
headers (
	int             fdi,		/* input image file descriptor	 */
	double	       *bline,		/* input image UL geo line coord */
	double	       *bsamp,		/* input image UL geo samp coord */
	double	       *dline,		/* input image geo line incr	 */
	double	       *dsamp)		/* input image geo samp incr	 */
{
	int		nbands;		/* # bands in images		 */
	BIH_T         **i_bih;		/* -> BIH of input image	 */
	GEOH_T        **i_geoh;		/* -> GEOH of input image	 */

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_geoh, 0};



   /* read input image BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* single-band images only */

	nbands = bih_nbands (i_bih[0]);
	if (nbands > 1) {
		error ("single-band mask input images only");
	}

   /* ingest input image GEOH */

	gethdrs (fdi, request, NO_COPY, ERROR);

   /* make sure image has geo header */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) != NULL) {
		*bline = geoh_bline (i_geoh[0]);
		*bsamp = geoh_bsamp (i_geoh[0]);
		*dline = geoh_dline (i_geoh[0]);
		*dsamp = geoh_dsamp (i_geoh[0]);
	} else {
		error ("input image has no geodetic header");
	}
}
