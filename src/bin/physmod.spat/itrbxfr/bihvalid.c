/******************************************************************************
**
** NAME
** 	bihvalid -- test if trbxfr input image is valid
** 
** SYNOPSIS
** 	int bihvalid (bih, nb)
**	BIH_T **bih;
**	int nb;
** 
** DESCRIPTION
** 	bihvalid tests if the given BIH is valid as an input image for the
**	turbulent transfer (trbxfr) model - if it has the correct number
**	of bands.
** 
** RETURN VALUE
** 	TRUE if given BIH is valid, FALSE otherwise
** 
*/

#include "ipw.h"
#include "bih.h"

bool_t
bihvalid(
	BIH_T         **bih,		/* -> BIH		 */
	int             nb)		/* valid # bands	 */
{

   /* make sure input image has exactly nb bands */

	if (bih_nbands(bih[0]) != nb) {
		usrerr("input file has %d bands", bih_nbands(bih[0]));
		return (FALSE);
	}

	return (TRUE);
}
