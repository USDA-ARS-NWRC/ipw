/*
** NAME
**	headers -- read/write headers
**
** SYNOPSIS
**	headers (fdi, fdm, fdo, nbits)
**	int fdi;
**	int fdm;
**	int fdo;
**	double elev;
**	double tau;
**	double S0;
**	int *nbits;
**
** DESCRIPTION
**	headers reads the headers of the input and mask images and c
**	write the BIH header of the output image.
**
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "fpio.h"
#include "pgm.h"

void
headers (
	int             fdi,		/* input file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output file descriptor	 */
	int            *nbits)		/* # output bits		 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **m_bih;		/* mask BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* ingest BIH of input image */

	if ((i_bih = bihread(fdi)) == NULL)
		error ("error reading BIH of input image");

   /* ingest BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL)
			error ("error reading BIH of mask image");
		if (bih_nlines(i_bih[0]) != bih_nlines(m_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]))
			error ("mask and input images have different dimensions");
		if (bih_nbands(m_bih[0]) != 1)
			error ("mask image must have only one band");
		skiphdrs (fdm);
	}

   /* set output nbits from input image, if needed */

	if (nbits[0] == 0)
		nbits[0] = hnbits(fdi, 0);
	if (nbits[1] == 0)
		nbits[1] = nbits[0];

	if (hnbands(fdi) != IBANDS)
		error ("input image must have exactly %d bands", IBANDS);

   /* write output BIH */

	o_bih = (BIH_T **)
		hdralloc (OBANDS, sizeof(BIH_T *), fdo, BIH_HNAME);
	assert (o_bih != NULL);
	o_bih[0] = bihmake (0, nbits[0],
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			   OBANDS);
	assert (o_bih[0] != NULL);
	o_bih[1] = bihmake (0, nbits[1],
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			   OBANDS);
	if  (bihwrite (fdo, o_bih) == ERROR) {
		error ("can't write new BIH");
	}


   /* ingest LQH */

	gethdrs (fdi, request, OBANDS, fdo);

}
