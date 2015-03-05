/*
** NAME
**      headers -- read headers for imgsum
** 
** SYNOPSIS
**      void headers (fdi, fdm)
**      int fdi, fdm;
** 
** DESCRIPTION
**      headers reads and checks the headers of the given input image
**      and mask image, if specified.
**
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"
#include "pgm.h"


void
headers (
	int		fdi,		/* input image file descriptor	 */
	int		fdm)		/* mask image file descriptor	 */
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **m_bihpp;	/* -> mask BIH array		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, NULL};


   /* read input BIH */

	i_bihpp = bihread (fdi);
	if (i_bihpp == NULL) {
		error ("can't read basic image header");
	}

   /* check input BIH */

	if (bih_nbands (i_bihpp[0]) > 1) {
		error ("image has %d bands; single band images only",
		       bih_nbands (i_bihpp[0]));
	}

   /* process LQH header */

	gethdrs (fdi, request, NO_COPY, ERROR);


   /* process mask image headers */

	if (fdm != ERROR) {
		if ((m_bihpp = bihread (fdm)) == NULL) {
			error ("can't read mask basic image header");
		}
		skiphdrs (fdm);
		if (hnbands(fdm) != 1) {
			error ("mask image has multiple bands");
		}
		if (bih_nlines(i_bihpp[0]) != bih_nlines(m_bihpp[0]) ||
		    bih_nsamps(i_bihpp[0]) != bih_nsamps(m_bihpp[0])) {
			error ("input and mask images have different dimensions");
		}
	}
}
