/*
** NAME
** 	headers -- read/write headers for import program
** 
** SYNOPSIS
**	headers (fdo, nlines, nsamps, nbands, nbits);
** 	int fdo;
**	int nlines, nsamps, nbands;
**	int *nbits;
** 
** DESCRIPTION
** 	Headers writes the output basic image header for the output image
**	of the import program.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"

void
headers (
	int             fdo,		/* output image file descriptor	 */
	int		nlines,		/* # lines in output image	 */
	int		nsamps,		/* # samps in output image	 */
	int		nbands,		/* # bands in output image	 */
	int	       *nbits)		/* # bits/band in output image	 */
{
	BIH_T         **o_bih;		/* output BIH			 */
	int		band;		/* band loop counter		 */


   /* create output BIH */

	o_bih = (BIH_T **) hdralloc (nbands, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL) {
		error ("Can't allocate new BIH");
	}

	o_bih[0] = bihmake (0, nbits[0], (STRVEC_T *) NULL, NULL,
			   (BIH_T *) NULL, nlines, nsamps, nbands);
	if (o_bih[0] == NULL) {
		error ("Can't make new BIH");
	}

	for (band = 1; band < nbands; band++) {
		o_bih[band] = bihmake (0, nbits[band], (STRVEC_T *) NULL,
			   	    NULL, o_bih[0], nlines, nsamps, nbands);
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}
}
