/*
**
** NAME
** 	precip_hdrs -- read input or precip image headers
** 
** SYNOPSIS
** 	#include "pgm.h"
**
**	void
**	precip_hdrs(
**	    char       *filename)	|* name of image (for messages)  *|
** 
** DESCRIPTION
** 	precip_hdrs reads and checks the headers of the current precipitation
**	image.
** 
** GLOBAL VARIABLES READ
**	fdp
**	nlines
**	nsamps
** 
** GLOBAL VARIABLES MODIFIED
*/

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"
#include "pgm.h"

UNITS_T precip_units[] = {
	U_KILOGRAMS_PER_SQUARE_METER,
	U_PERCENT,
	U_KILOGRAMS_PER_CUBIC_METER,
	U_CELSIUS
};


void
precip_hdrs(
	char	       *filename)	/* name of image (for messages)  */
{
	BIH_T         **p_bih;		/* -> precip image BIH		 */
 
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH of image */

	if ((p_bih = bihread(fdp)) == NULL) {
		error("can't read BIH of file %s", filename);
	}

   /* check input BIH */

	if (hnbands(fdp) != PBANDS) {
		error("precip image %s should have %d bands", filename, PBANDS);
	}

	if ((bih_nlines(p_bih[0]) != nlines) ||
	    (bih_nsamps(p_bih[0]) != nsamps))
		error("precip image %s has the wrong dimensions", filename);

   /* ingest LQH, other headers are ignored */
   /* call gethdrs instead of fphdrs, so we can access the LQ headers (JBD,
      Nov 95) */
 
	gethdrs(fdp, request, NO_COPY, ERROR);
 
   /* check the units of the LQ headers */
	if (units_warn)
		check_units((LQH_T **) hdr_addr(h_lqh), precip_units, PBANDS,
			    fdp);
}
