/*
** NAME
** 	headers -- read input headers/write output headers for iinterp
** 
** SYNOPSIS
**	headers (fdi, fdo, nbits)
** 	int fdi, fdo;
**	int nbits;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the IPW input image
**	and initializes and writes headers for the output image
**	for the ftransrorm program.
*/

#include "ipw.h"
#include "pgm.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"

void
headers (
	int		fdi,		/* input image file descriptor	 */
	int		fdo,		/* output image file descriptor  */
	int		nbits)		/* # bits in output image	 */
{
	BIH_T          **i_bih;		/* -> input BIH			*/
	BIH_T          **o_bih;		/* -> output BIH		*/

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* check input BIH */

	if (hnbands(fdi) != 1) {
		error ("image has %d bands; single-band images only",
			hnbands(fdi));
	}

   /* create output BIH, same # bits as input image */

	o_bih = (BIH_T **) hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);

	if (nbits <= 0)
		nbits = bih_nbits (i_bih[0]);

	o_bih[0] = bihmake (0, nbits,
			   bih_history(i_bih[0]), bih_annot(i_bih[0]), o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]), 1);
	if (o_bih[0] == NULL) {
		error ("Can't create BIH");
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest LQH, other headers get passed thru */

	gethdrs (fdi, request, 1, fdo);

}
