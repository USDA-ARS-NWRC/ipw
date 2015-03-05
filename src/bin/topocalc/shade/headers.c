/*
 * process headers
 */

#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "lqh.h"
#include "sunh.h"

#include "pgm.h"

void
headers(
	int             fdi,		/* input image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	double          ctheta,		/* cosine solar zenith		 */
	double          azimuth)	/* solar azimuth, radians	 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	SUNH_T        **o_sunh;		/* output SUNH			 */
	fpixel_t       *fpmax;

 /* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

 /*
  * read BIH
  */
	if ((i_bih = bihread(fdi)) == NULL) {
		error("can't read BIH");
	}
 /*
  * must be two bands in input slope/azimuth file
  */
	if (hnbands(fdi) != 2) {
		error("input slope/azimuth file must have 2 bands");
	}
 /*
  * create output BIH
  */
 /* NOSTRICT */
	o_bih = (BIH_T **) hdralloc(1, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL) {
		error("Can't make new BIH");
	}
	o_bih[0] = bihmake(bih_nbytes(i_bih[0]), bih_nbits(i_bih[0]),
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   (BIH_T *) NULL,
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]), 1);
	if (o_bih[0] == NULL) {
		error("Can't make new BIH");
	}
 /*
  * write output BIH
  */
	if (bihwrite(fdo, o_bih) == ERROR) {
		error("Can't write output BIH");
	}
 /*
  * ingest LQH; other headers get passed thru
  */
	gethdrs(fdi, request, hnbands(fdo), fdo);
 /*
  * check that we read LQH
  */
	if (hdr_addr(h_lqh) == NULL) {
		warn("input file has no LQH");
	}
 /*
  * set global values to slope, aspect; first band must be slope
  */
	fpmax = fpfmax(fdi);
	if (fpmax[0] != 1) {
		warn("band 0 of input not slope");
	}
 /*
  * output LQH
  */
	if ((o_lqh = newlqh(fdi, fdo)) == NULL) {
		error("Can't create new LQH");
	}
 /*
  * write output LQH
  */
	if (lqhwrite(fdo, o_lqh) == ERROR) {
		error("Can't write output LQH");
	}
 /*
  * output SUNH
  */
	if ((o_sunh = newsunh(fdo, ctheta, azimuth)) == NULL) {
		error("Can't create new SUNH");
	}
 /*
  * write output SUNH
  */
	if (sunhwrite(fdo, o_sunh) == ERROR) {
		error("Can't write output SUNH");
	}
 /*
  * get ready for rest of output
  */
	if (boimage(fdo) == ERROR) {
		error("Can't terminate header output");
	}

	SAFE_FREE(o_sunh[0]);
	SAFE_FREE(o_sunh);
}
