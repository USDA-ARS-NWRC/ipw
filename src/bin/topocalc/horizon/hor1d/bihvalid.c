/*
 * is this BIH OK?
 */

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

bool_t
bihvalid(
	BIH_T         **bihpp,		/* -> BIH		 */
	int             nb)		/* valid # bands	 */
{
 /*
  * make sure input image is only nb band
  */
	if (bih_nbands(bihpp[0]) != nb) {
		usrerr("input file has %d bands", bih_nbands(bihpp[0]));
		return (FALSE);
	}

 /*
  * make sure input image has spacing
  */
	if (bih_nlines(bihpp[0]) < 2) {
		usrerr("only %ld line in input image", bih_nlines(bihpp[0]));
		return (FALSE);
	}

	if (bih_nsamps(bihpp[0]) < 2) {
		usrerr("only %ld samp in input image", bih_nsamps(bihpp[0]));
		return (FALSE);
	}

	return (TRUE);
}
