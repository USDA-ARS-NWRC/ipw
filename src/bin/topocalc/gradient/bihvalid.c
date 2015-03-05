/*
 * is this BIH OK?
 */

#include "ipw.h"
#include "bih.h"

bool_t
bihvalid(
	BIH_T         **bih,		/* -> BIH		 */
	int             nb)		/* valid # bands	 */
{
 /*
  * make sure input image is only nb band
  */
	if (bih_nbands(bih[0]) != nb) {
		usrerr("input file has %d bands", bih_nbands(bih[0]));
		return (FALSE);
	}
 /*
  * make sure input image has spacing
  */
	if (bih_nlines(bih[0]) < 2) {
		usrerr("only %d line in input image", bih_nlines(bih[0]));
		return (FALSE);
	}
 /*
  * make sure input image lines have at least two pixels
  */
	if (bih_nsamps(bih[0]) < 2) {
		usrerr("only %d samp in input image", bih_nsamps(bih[0]));
		return (FALSE);
	}

	return (TRUE);
}
