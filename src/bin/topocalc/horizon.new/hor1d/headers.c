/*
 * process headers, variables in common with main part of routine
 * are statically declared above main part
 */

#include <math.h>
#include <errno.h>

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "geoh.h"
#include "horh.h"
#include "lqh.h"
#include "skewh.h"


void
headers(void)
{
	BIH_T         **i_bih;		/* -> input BIH			 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	LQH_T         **o_lqh;		/* -> output LQH		 */
	GEOH_T        **i_geoh;		/* -> input GEOH		 */
	SKEWH_T       **i_skew;		/* -> input SKEWH		 */
	GEOH_T        **o_geoh;		/* -> output GEOH		 */
	HORH_T        **o_horh;		/* -> output HORH		 */

	/* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};

	/* NOSTRICT */
	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};

	/* NOSTRICT */
	static GETHDR_T h_skew = {SKEWH_HNAME, (ingest_t) skewhread};
	static GETHDR_T *request[] = {&h_lqh, &h_geo, &h_skew, 0};

	o_geoh = NULL;
	/*
	 * read BIH
	 */

	if ((i_bih = bihread(parm.i_fd)) == NULL) {
		error("can't read BIH");
	}

	/*
	 * check if valid input elevation file
	 */

	if (!bihvalid(i_bih, 1)) {
		error("BIH invalid");
	}

	/*
	 * create output BIH
	 */
	/* NOSTRICT */
	if ((o_bih = (BIH_T **) hdralloc(1, sizeof(BIH_T *),
			parm.o_fd, BIH_HNAME))
			== NULL)
		error("can't allocate headers");
	if ((o_bih[0] = bihmake(0, parm.nbits, bih_history(i_bih[0]),
			bih_annot(i_bih[0]), (BIH_T *) NULL,
			bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			1))
			== NULL)
		error("can't make BIH");
	/*
	 * write output BIH
	 */

	if (bihwrite(parm.o_fd, o_bih) == ERROR) {
		error("can't write BIH");
	}

	/*
	 * ingest LQH and GEOH; other headers get passed thru
	 */

	gethdrs(parm.i_fd, request, 1, parm.o_fd);

	if (!got_hdr(h_lqh))
		warn("input file has no LQH, raw values used");

	/*
	 * get spacing if geodetic header; output GEOH is same as input
	 */
	if (got_hdr(h_geo)) {
		/* NOSTRICT */
		i_geoh = (GEOH_T **) hdr_addr(h_geo);
		if (parm.spacing != 0)
			warn("spacing in geodetic header ignored");
		else {
			parm.spacing = geohspace(i_geoh[0]);
			if (errno)
				error("bad return from geohspace");
		}
		/* NOSTRICT */
		o_geoh = (GEOH_T **) hdralloc(1, sizeof(GEOH_T *),
				parm.o_fd, GEOH_HNAME);
		o_geoh[0] = i_geoh[0];
		if (geohwrite(parm.o_fd, o_geoh) == ERROR)
			error("can't write GEOH");
	}
	else {
		if (parm.spacing == 0) {
			warn("no geodetic header, spacing set to 1.0");
			parm.spacing = 1;
		}
	}
	/*
	 * adjust spacing if input file has been skewed [divide by cosine of skew
	 * angle (original is in degrees)]
	 */
	if (got_hdr(h_skew)) {
		/* NOSTRICT */
		i_skew = (SKEWH_T **) hdr_addr(h_skew);
		parm.spacing /= cos(skewh_angle(i_skew[0]) * atan(1.) / 45);
		/*
		 * pass skew header to output
		 */
		if (skewhwrite(parm.o_fd, i_skew) == ERROR) {
			error("can't write SKEWH");
		}
	}
	/*
	 * output LQH, if output file not a mask
	 */
	if (parm.nbits != 1) {
		if ((o_lqh = newlqh(parm.o_fd)) == NULL) {
			error("Can't create new LQH");
		}

		/*
		 * write output LQH, if output file not a mask
		 */
		if (lqhwrite(parm.o_fd, o_lqh) == ERROR) {
			error("Can't write output LQH");
		}
	}
	/*
	 * output HORH
	 */
	/* NOSTRICT */
	o_horh = (HORH_T **) hdralloc(1, sizeof(HORH_T *),
			parm.o_fd, HORH_HNAME);
	assert(o_horh != NULL);
	assert((o_horh[0] = horhmake(parm.azimuth)) != NULL);
	/*
	 * write output HORH
	 */
	if (horhwrite(parm.o_fd, o_horh) == ERROR) {
		error("can't write output HORH");
	}

	/*
	 * get ready for rest of output
	 */

	if (boimage(parm.o_fd) == ERROR) {
		error("Can't terminate header output");
	}

	SAFE_FREE(o_horh[0]);
	SAFE_FREE(o_horh);

	if (got_hdr(h_geo)) {
		SAFE_FREE(o_geoh);
	}
}
