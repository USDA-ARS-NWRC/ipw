/******************************************************************************
**
** NAME
** 	headers -- read input and elevation image headers and write 
**		   output image header
** 
** SYNOPSIS
** 
**	void headers (fdi, fdm, fd_elev, inv, fdo)
**	int fdi, fd_elev, fdo, fdm;
**	int inv;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the input images and
**	creates and writes headers for the output image for the potential
**	temperature program pott
** 
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pgm.h"

static char units[] = "degrees C";

void
headers (
	int             fdi,		/* input image file descriptor	*/
	int             fdm,		/* mask image file descriptor	*/
	int             fd_elev,	/* elev image file descriptor	*/
	int		inv,		/* inversion flag		*/
	int             fdo)		/* output image file descriptor	*/

{
	BIH_T         **i_bih;		/* -> input BIH			*/
	BIH_T         **e_bih;		/* -> elevation BIH		*/
	BIH_T         **m_bih;		/* -> mask BIH			*/
	BIH_T         **o_bih;		/* -> output BIH		*/
	LQH_T         **o_lqh;		/* -> output LQH		*/
	pixel_t		ibkpt[2];	/* integer breakpoints for LQH  */
	fpixel_t	fbkpt[2];	/* floating breakpoints for LQH */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH of input images */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}
	if ((e_bih = bihread (fd_elev)) == NULL) {
		error ("can't read elevation BIH");
	}
	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read mask BIH");
		}
		skiphdrs (fdm);
	}

   /* check input and elevation BIH */

	if (hnbands(fdi) != 1) {
		error ("not valid BIH for this program");
	}
	if (hnbands(fd_elev) != 1) {
		error ("not valid elevation BIH for this program");
	}
	if (hnsamps(fdi) != hnsamps(fd_elev) ||
	    hnlines(fdi) != hnlines(fd_elev)) {
		error ("input and elevation images have different dimensions");
	}

	if (fdm != ERROR) {
		if (hnsamps(fdi) != hnsamps(fdm) ||
	    	    hnlines(fdi) != hnlines(fdm)) {
			error ("input and mask images have different dimensions");
		}
		if (hnbands(fdm) != 1)
			error ("mask image has more than one band");
	}

   /* create output BIH, # bytes will be set by in image */

	o_bih = (BIH_T **) hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);
	o_bih[0] = bihmake (0, bih_nbits(i_bih[0]),
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
	fphdrs (fd_elev, NO_BAND, ERROR);

	if (hdr_addr (h_lqh) == NULL)
		warn ("input file has no LQH; raw values used");

   /* allocate LQH */

	o_lqh = (LQH_T **) hdralloc (1, sizeof(LQH_T *), fdo, LQH_HNAME);

   /* create LQH  */

	ibkpt[0] = 0;
	ibkpt[1] = ipow2 (hnbits(fdo, 0)) - 1;
	fbkpt[0] = T_MIN;
	fbkpt[1] = T_MAX;
	o_lqh[0] = lqhmake (bih_nbits(i_bih[0]), 2, ibkpt, fbkpt, units, (char *) NULL);
	if (o_lqh[0] == NULL) {
		error ("Can't create LQH");
	}

   /* write new LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
