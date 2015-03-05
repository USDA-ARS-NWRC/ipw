/*
** NAME
** 	headers -- read headers
** 
** DESCRIPTION
** 	headers reads and checks the headers of the given input image
**	and mask image, if specified.
** 
*/

#include "pgm.h"

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "fpio.h"


void
headers (
	int		fdi,		/* input image file descriptor	 */
	int		fdm)		/* mask image file descriptor	 */
{
	BIH_T	      **i_bihpp;	/* input image BIH		 */
	int		nlines;		/* #lines in input image	 */
	int		nsamps;		/* #samps in input image	 */
	int		nbands;		/* #bands in input image	 */


   /* read BIH */

	i_bihpp = bihread (fdi);
	if (i_bihpp == NULL) {
		error ("can't read basic image header");
	}

	nlines = bih_nlines (i_bihpp[0]);
	nsamps = bih_nsamps (i_bihpp[0]);
	nbands = bih_nbands (i_bihpp[0]);
	if (nbands > 1) {
		error ("image has %d bands; single band images only",
		       nbands);
	}

   /* ingest floating point header; ignore all others */

	fphdrs (fdi, NO_COPY, ERROR);

   /* process mask image headers */

	if (fdm != ERROR) {
		if (bihread (fdm) == NULL) {
			error ("can't read mask basic image header");
		}
		skiphdrs (fdm);
		if (hnbands(fdm) != 1) {
			error ("mask image has multiple bands");
		}
		if (nlines != hnlines(fdm) ||
		    nsamps != hnsamps(fdm)) {
			error ("input image and mask image have different dimensions");
		}
	}
}
