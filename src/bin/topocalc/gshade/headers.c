/*
** NAME
** 	headers -- read/write headers for gshade program
** 
** SYNOPSIS
**	headers (fdi, fds, fdm, fdo, zenith)
** 	int fdi, fds, fdm, fdo;
**	bool_t zenith;
** 
** DESCRIPTION
** 	Headers reads/processed the headers of the input images and writes
**	the headers for the output image of the gshade program.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "lqh.h"
#include "pgm.h"

void
headers (
	int             fdi,		/* input image file descriptor	 */
	int             fds,		/* solar image file descriptor	 */
	int             fdm,		/* mask image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	bool_t		zenith)		/* flag if fsd contains zenith	 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **s_bih;		/* solar BIH			 */
	BIH_T         **m_bih;		/* mask BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	fpixel_t       *fpmin;		/* min value in F.P. map	 */
	fpixel_t       *fpmax;		/* max value in F.P. map	 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T sh_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};
	static GETHDR_T *srequest[] = {&sh_lqh, 0};


   /* read BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read BIH of input image");
	}

   /* must be two bands in input slope/azimuth file */

	if (hnbands(fdi) != 2) {
		error ("input slope/azimuth file must have 2 bands");
	}

   /* read solar BIH */

	if ((s_bih = bihread(fds)) == NULL) {
		error ("can't read BIH of solar image");
	}

   /* must be two bands in solar image */

	if (hnbands(fdi) != 2) {
		error ("input solar image must have 2 bands");
	}

   /* make sure input and solar images have same dimensions */

	if (bih_nsamps(i_bih[0]) != bih_nsamps(s_bih[0]) ||
   	    bih_nlines(i_bih[0]) != bih_nlines(s_bih[0])) {
		error ("input and solar images have different dimensions");
	}

   /* read mask BIH */
   /* make sure it has same dimensions of input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]) ||
   	    	    bih_nlines(i_bih[0]) != bih_nlines(m_bih[0])) {
			error ("input and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* create output BIH */

	o_bih = (BIH_T **) hdralloc(1, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL) {
		error ("Can't make new BIH");
	}
	o_bih[0] = bihmake (bih_nbytes(i_bih[0]), bih_nbits(i_bih[0]),
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   (BIH_T *) NULL,
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]), 1);
	if (o_bih[0] == NULL) {
		error ("Can't make new BIH");
	}

   /* write output BIH */

	if (bihwrite(fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest LQHs; other headers get passed thru */

	gethdrs (fdi, request, hnbands(fdo), fdo);
	gethdrs (fds, srequest, NO_COPY, ERROR);

   /* check that we read LQHs */

	if (hdr_addr(h_lqh) == NULL) {
		warn ("input file has no LQH");
	}
	if (hdr_addr(sh_lqh) == NULL) {
		warn ("solar file has no LQH");
	}

   /* set global values to slope, aspect; first band must be slope */

	fpmin = fpfmin (fdi);
	fpmax = fpfmax (fdi);
	if (fpmin[0] < -1.0 || fpmax[0] > 1.0) 
		error ("band 0 of input image not slope");
	if (fpmin[1] < -180.0 || fpmax[1] > 180.0)
		error ("band 1 of input image not exposure");

   /* check solar image */

	fpmin = fpfmin (fds);
	fpmax = fpfmax (fds);
	if (zenith) {
		if (fpmin[0] < -180.0 || fpmax[0] > 180.0)
			error ("band 0 of solar image not azimuth");
	} else {
		if (fpmin[0] < -1.0 || fpmax[0] > 1.0)
			error ("band 0 of solar image not cos zenith angle");
	}
	if (fpmin[1] < -180.0 || fpmax[1] > 180.0)
		error ("band 1 of solar image not azimuth");

   /* output LQH */

	if ((o_lqh = newlqh (fdi, fdo)) == NULL) {
		error ("Can't create new LQH");
	}

   /* write output LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for rest of output */

	if (boimage (fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
