/*
 * process headers
 */

#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "geoh.h"
#include "horh.h"

#include "pgm.h"

void
headers(void)
{
	BIH_T         **h_bih;		/* input horizon BIH		 */
	BIH_T         **o_bih;		/* output BIH			 */
	BIH_T         **s_bih;		/* input slope/azm BIH		 */
	HORH_T        **horh;		/* input HORH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	fpixel_t       *fpmax;
	int             band;

 /* NOSTRICT */
	static GETHDR_T s_lqh = {LQH_HNAME, (ingest_t) lqhread};

 /* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};

 /* NOSTRICT */
	static GETHDR_T h_hh = {HORH_HNAME, (ingest_t) horhread};

#if 0
 /* NOSTRICT */
	static GETHDR_T h_geo = {GEOH_HNAME, NULL};

#endif
	static GETHDR_T *sreq[] = {&s_lqh, 0};
	static GETHDR_T *hreq[] = {&h_lqh, &h_hh,
#if 0
		&h_geo,
#endif
	0};

 /*
  * read BIH's
  */
	if ((s_bih = bihread(parm.i_fds)) == NULL ||
	    (h_bih = bihread(parm.i_fdh)) == NULL) {
		error("can't read BIH's");
	}
 /*
  * must be two bands in input slope/azimuth file
  */
	if (bih_nbands(s_bih[0]) != 2) {
		error("input slope/azimuth file must have 2 bands");
	}
 /*
  * create output BIH
  */
 /* NOSTRICT */
	o_bih = (BIH_T **) hdralloc(2, sizeof(BIH_T *), parm.o_fd, BIH_HNAME);
	if (o_bih == NULL) {
		error("Can't make new BIH");
	}
	o_bih[0] = bihmake(bih_nbytes(s_bih[0]), bih_nbits(s_bih[0]),
			   bih_history(s_bih[0]), bih_annot(s_bih[0]),
			   (BIH_T *) NULL,
		       bih_nlines(s_bih[0]), bih_nsamps(s_bih[0]), 2);
	if (o_bih[0] == NULL) {
		error("Can't make new BIH");
	}
	o_bih[1] = bihmake(bih_nbytes(s_bih[0]), bih_nbits(s_bih[0]),
			   bih_history(s_bih[0]), bih_annot(s_bih[0]),
			   o_bih[0],
			   bih_nlines(s_bih[0]), bih_nsamps(s_bih[0]), 2);
	if (o_bih[1] == NULL) {
		error("Can't make new BIH");
	}
 /*
  * write output BIH
  */
	if (bihwrite(parm.o_fd, o_bih) == ERROR) {
		error("Can't write output BIH");
	}
 /*
  * ingest slope/azm LQH; other headers get passed thru
  */
	gethdrs(parm.i_fds, sreq, bih_nbands(o_bih[0]), parm.o_fd);
	if (!got_hdr(s_lqh)) {
		error("slope/azm image has no LQH");
	}
 /*
  * ingest horizon LQH and HORH; other headers get ignored
  */
	gethdrs(parm.i_fdh, hreq, NO_COPY, parm.o_fd);
	if (!got_hdr(h_lqh)) {
		error("horizon image has no LQH");
	}
	if (!got_hdr(h_hh)) {
		error("horizon image has no HORH");
	}
 /* NOSTRICT */
	horh = (HORH_T **) hdr_addr(h_hh);
 /*
  * put horizon azimuths in external buffer
  */
 /* NOSTRICT */
	parm.hazm = (float *) ecalloc(bih_nbands(h_bih[0]), sizeof(float));
	assert(parm.hazm != NULL);
	for (band = bih_nbands(h_bih[0]) - 1; band >= 0; --band) {
		parm.hazm[band] = horh_azm(horh[band]);
	}
 /*
  * first band must be slope
  */
	fpmax = fpfmax(parm.i_fds);
	if (fpmax[0] != 1) {
		error("band 0 of input not slope");
	}

 /*
  * output LQH
  */
	if ((o_lqh = newlqh(parm.i_fds, parm.o_fd)) == NULL) {
		error("Can't create new LQH");
	}
 /*
  * write output LQH
  */
	if (lqhwrite(parm.o_fd, o_lqh) == ERROR) {
		error("Can't write output LQH");
	}
 /*
  * get ready for rest of output
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("Can't terminate header output");
	}
}
