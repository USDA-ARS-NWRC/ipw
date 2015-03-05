/*
**
** NAME
** 	headers -- read input image header and write output image header
** 
** SYNOPSIS
** 
**	void headers (fdi, fdm, nbits, fdo)
**	int fdi, fdm, fdo;
**	int nbits;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the input image and
**	creates and writes headers for the output image of the irh2vp program.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pgm.h"

#define NBITS	16

void
headers (
	int             fdi,		/* input image file descriptor	*/
	int             fdm,		/* mask image file descriptor	*/
	int		nbits,		/* # bits in output image	*/
	int             fdo)		/* output image file descriptor	*/
{
	BIH_T         **i_bih;		/* -> input BIH			*/
	BIH_T         **m_bih;		/* -> mask BIH			*/
	BIH_T         **o_bih;		/* -> output BIH		*/

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* check input BIH */

	if (bih_nbands(i_bih[0]) != IBANDS) {
		error ("image has %d bands; 2 band images only",
			bih_nbands(i_bih[0]));
	}

   /* read BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read mask BIH");
		}

		if (bih_nlines(i_bih[0]) != bih_nlines(m_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]))
			error ("mask and input images are incompatible");

		if (bih_nbands(m_bih[0]) != 1)
			error ("mask image has > 1 bands");

		skiphdrs (fdm);
	}

   /* create output BIH, # bytes will be set by bihmake */

	if (nbits <= 0)
		nbits = NBITS;

	o_bih = (BIH_T **) hdralloc (OBANDS, sizeof(BIH_T *), fdo, BIH_HNAME);
	o_bih[0] = bihmake (0, nbits,
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   NULL,
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			   OBANDS);
	if (o_bih[0] == NULL) {
		error ("error creating output BIH");
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest LQH, other headers get passed thru */

	gethdrs (fdi, request, OBANDS, fdo);

	if (hdr_addr (h_lqh) == NULL)
		warn ("input file has no LQH; raw values used");

}
