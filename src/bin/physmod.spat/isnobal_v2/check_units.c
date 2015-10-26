/*
**
** NAME
** 	check_units -- check the units fields of an array of LQ headers
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	check_units(
**	    LQH_T     **lq_headers,	|* LQ headers for each band	 *|
**	    UNITS_T    *units,		|* units ids for each band	 *|
**	    int		nbands,		|* # of bands 			 *|
**	    int	        fd)		|* file descriptor of image 	 *|
** 
** DESCRIPTION
** 	check_units scans an array of LQ headers to make sure the units
**	field matches a specific value.  {lq_headers} is an array of
**	pointers to LQ headers.  {units} is the corresponding array of
**	expected units ids (see include file units.h).  {nbands} is the # of
**	bands, i.e., the size of both arrays.  {fd} is the file descriptor
**	of the image whose bands are being checked.
*/

#include "ipw.h"
#include "pgm.h"

void
check_units(
	LQH_T         **lq_headers,	/* LQ headers for each band	 */
	UNITS_T        *units,		/* units ids for each band	 */
	int		nbands,		/* # of bands 			 */
	int	        fd)		/* file descriptor of image 	 */
{
	int             i;


   /* check units field of each LQ header */

	for (i = 0; i < nbands; i++) {
	    if (lqh_units(lq_headers[i]) == NULL) {
		uferr(fd);
		warn("Units for band %d are missing; expected \"%s\"",
		     i, units_as_str(units[i]));
	    }
	    else if (! units_match(lqh_units(lq_headers[i]), units[i])) {
		uferr(fd);
		warn("Units for band %d are \"%s\"; expected \"%s\"", i,
		     lqh_units(lq_headers[i]), units_as_str(units[i]));
	    }
	}
}
