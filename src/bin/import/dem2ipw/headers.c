#include <math.h>
#include <string.h>

#include "ipw.h"

#include "bih.h"
#include "fpio.h"
#include "geoh.h"
#include "gethdrs.h"
#include "lqh.h"
#include "orh.h"

#include "pgm.h"

/*
 * headers -- create and write image headers
 */

void
headers(void)
{
	BIH_T         **o_bihpp;	/* -> output BIH array		 */
	ORH_T         **o_orhpp;	/* -> output ORH array		 */
	GEOH_T        **o_geohpp;	/* -> output GEOH array		 */
	LQH_T         **o_lqhpp =NULL;	/* -> output LQH array		 */
	int             nsamps;		/* # samples in longest line	 */
	int             nlines;		/* # profiles			 */
	int             nbits =0;	/* # bits per pixel		 */
	pixel_t         ival[3];	/* break-points for LQH		 */
	fpixel_t        fval[3];	/* elevations at break-points	 */
	double          bline;		/* GEOH coords for first line	 */
	double          bsamp;		/* GEOH coords for first sample	 */
	double          dline;		/* spacing between lines	 */
	double          dsamp;		/* spacing between samples	 */
	LQH_T         **i_lqhpp =NULL;	/* -> input LQH                 */

 /* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

 /*
  * read LQH supplied, if there is one
  */
	if (parm.islqh) {
		if (bihread(parm.h_fd) == NULL) {
			error("can't read BIH of supplied LQH");
		}
		gethdrs(parm.h_fd, request, NO_COPY, parm.o_fd);
 /* NOSTRICT */
		i_lqhpp = (LQH_T **) hdr_addr(h_lqh);
		nbits = hnbits(parm.h_fd, 0);
	}
 /*
  * Create and write BIH. Set nbits large enough to hold elevations
  * accurate to the input quantization, unless LQH header is provided
  * as input
  */
	nsamps = 1 + (parm.nmax - parm.nmin) / parm.dely;
	if (nsamps < parm.maxelevs) {
		nsamps = parm.maxelevs;
	}
	nlines = parm.nprofls;

	if (!parm.islqh || i_lqhpp == NULL) {
		if (parm.zmin < 0) {
			nbits = hbit((unsigned) (1 + parm.zmax - parm.zmin));
		}
		else {
			nbits = hbit((unsigned) parm.zmax);
		}
	}
 /* NOSTRICT */
	o_bihpp = (BIH_T **) hdralloc(1, sizeof(BIH_T *), parm.o_fd,
				      BIH_HNAME);
	o_bihpp[0] = bihmake(0, nbits, (STRVEC_T *) NULL,
			     (STRVEC_T *) NULL, (BIH_T *) NULL,
			     nlines, nsamps, 1);
	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * orientation header; USGS DEM orientation is lower left,
  * column-major
  */
 /* NOSTRICT */
	o_orhpp = (ORH_T **) hdralloc(1, sizeof(ORH_T *), parm.o_fd,
				      ORH_HNAME);
	o_orhpp[0] = orhmake(COLUMN, ORIG_4);
	if (orhwrite(parm.o_fd, o_orhpp) == ERROR) {
		error("can't write orientation header");
	}
 /*
  * geodetic header
  */
	bline = parm.emin;
	bsamp = parm.nmin;
	dline = (parm.emax > parm.emin) ? parm.delx : -parm.delx;
	dsamp = (parm.nmax > parm.nmin) ? parm.dely : -parm.dely;

 /* NOSTRICT */
	o_geohpp = (GEOH_T **) hdralloc(1, sizeof(GEOH_T *), parm.o_fd,
					GEOH_HNAME);
	o_geohpp[0] = geohmake(bline, bsamp, dline, dsamp,
			 (char *) (parm.units), (char *) (parm.csys));
	if (geohwrite(parm.o_fd, o_geohpp) == ERROR) {
		error("can't write GEO header");
	}
 /*
  * Unless raw output desired or input linear quantization header
  * provided, determine from range of data (adjusting minimum to allow
  * for unique fill value).
  */
	if (parm.islqh && i_lqhpp != NULL) {
		if ((o_lqhpp = lqhdup(i_lqhpp, 1)) == NULL) {
			error("can't duplicate input LQH");
		}
		if (parm.units != NULL &&
		    (lqh_units(o_lqhpp[0]) = strdup(parm.units)) == NULL) {
			error("can't duplicate units string");
		}
	}
	else if (!parm.raw) {
		ival[0] = 0;
		ival[1] = ipow2(nbits) - 1;
		fval[0] = parm.zmin > 0 ? 0 : floor(parm.zmin - 1);
		fval[1] = parm.zmax;

 /* NOSTRICT */
		o_lqhpp = (LQH_T **) hdralloc(1, sizeof(LQH_T *), parm.o_fd,
					      LQH_HNAME);
		o_lqhpp[0] = lqhmake(nbits, 2, ival, fval,
				(char *) (parm.units), (char *) NULL);
	}
	if (parm.raw) {
		parm.fillval = 0;
	}
	else {
		if (lqhwrite(parm.o_fd, o_lqhpp) == ERROR) {
			error("can't write LQH header");
		}
		parm.fillval = fpfmin(parm.o_fd)[0];
	}
 /*
  * done
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}
}
