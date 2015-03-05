/*
** NAME
** 	headers -- read input headers/write output headers for classify
** 
** SYNOPSIS
**	headers (fdi, fdh, ibits, ibits_nbands, fdo);
** 	int fdi, fdh;
**	int *ibits;
**	int ibits_nbands;
**	int fdo;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the IPW input image
**	and initializes and writes headers for the output declassified
**	image for the declass program.
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "crh.h"
#include "lqh.h"
#include "pgm.h"

void
headers(
	int		 fdi,		/* input image file descriptor	*/
	int		 fdh,		/* input LQH file descriptor	*/
	int		*ibits,		/* #bits for each input band    */
	int		 ibits_nbands,	/* dimension of ibits array	*/
	int		 fdo)		/* output image file descriptor */
{
	int		*nbits;		/* # bits/band in output image  */
	int		 band;		/* band loop counter		*/
	int		 nbands;	/* # bands			*/
	BIH_T          **i_bih;		/* -> input BIH			*/
	BIH_T          **h_bih;		/* -> LQH BIH			*/
	CRH_T	       **i_crh;		/* -> input CRH			*/
	BIH_T          **o_bih;		/* -> output BIH		*/
	LQH_T          **o_lqh;		/* -> output LQH		*/

	static GETHDR_T h_crh = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_lqhh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_crh, &h_lqh, 0};
	static GETHDR_T *requesth[] = {&h_lqhh, 0};


   /* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}
	nbands = hnbands(fdi);

	nbits = (int *) ecalloc (nbands, sizeof(int));
	if (nbits == NULL)
		error ("Can't allocate nbits array");

	for (band = 0; band < nbands; band++) {
		if (band < ibits_nbands)
			nbits[band] = ibits[band];
		else
			nbits[band] = CHAR_BIT;
	} 

   /* read BIH input LQH file */
   /* duplicate supplied BIH */

	if (fdh != ERROR) {
		if ((h_bih = bihread (fdh)) == NULL) {
			error ("can't read BIH from input LQH");
		}
		if (hnbands(fdh) != hnbands(fdi)) {
			error ("input image and LQH have different #bands");
		}
		o_bih = bihdup (h_bih);

   /* create new BIH */

	} else {

   		/* create output BIH, # bytes will be set by bihmake */

		o_bih = (BIH_T **)hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);

		o_bih[0] = bihmake (0, nbits[0],
			   	    bih_history(i_bih[0]), bih_annot(i_bih[0]),
				    NULL, bih_nlines(i_bih[0]),
				    bih_nsamps(i_bih[0]), nbands);
		if (o_bih[0] == NULL) {
			error ("Can't create BIH");
		}
		for (band = 1; band < nbands; band++) {
			o_bih[band] = bihmake (0, nbits[0],
						bih_history(i_bih[0]),
						bih_annot(i_bih[0]),
				    		o_bih[0], bih_nlines(i_bih[0]),
				    		bih_nsamps(i_bih[0]), 1);
			if (o_bih[band] == NULL) {
				error ("Can't create BIH");
			}
		}
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest CRH, LQH, other headers get passed thru */

	gethdrs (fdi, request, 1, fdo);

   /* check for CRH */

	if ((i_crh = (CRH_T **) hdr_addr (h_crh)) == NULL)
		error ("input image has no CRH header");

   /* duplicate or create new LQH */

	if (fdh != ERROR) {
		gethdrs (fdh, requesth, NO_COPY, ERROR);
		o_lqh = lqhdup (lqh(fdh), nbands);
	} else {
		o_lqh = newlqh (nbands, nbits, i_crh, fdo);
	}

   /* write output LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}
}
