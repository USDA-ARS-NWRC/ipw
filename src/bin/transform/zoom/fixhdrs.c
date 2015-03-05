#include "ipw.h"

#include "geoh.h"
#include "gethdrs.h"
#include "winh.h"

#include "pgm.h"

/*
 * fixhdrs -- adjust spatial headers
 */

void
fixhdrs(void)
{
 /* NOSTRICT */
	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
 /* NOSTRICT */
	static GETHDR_T h_win = {WINH_HNAME, (ingest_t) winhread};

	static GETHDR_T *hv[] = {&h_geo, &h_win, NULL};

	double          zline;		/* line zoom factor		 */
	double          zsamp;		/* sample zoom factor		 */

 /*
  * ingest GEO and/or WIN headers; copy all others
  */
	gethdrs(parm.i_fd, hv, parm.nbands, parm.o_fd);

	if (!got_hdr(h_geo) && !got_hdr(h_win)) {
		return;
	}
 /*
  * compute spacing adjustment factors
  */
	zline = parm.skip_lines > 1 ?
		parm.skip_lines :
		1.0 / (double) parm.dup_lines;

	zsamp = parm.skip_samps > 1 ?
		parm.skip_samps :
		1.0 / (double) parm.dup_samps;
 /*
  * adjust GEO header
  */
	if (got_hdr(h_geo)) {
		int             band;	/* band counter			 */
		GEOH_T        **geohpp;	/* -> GEO header array		 */

 /* NOSTRICT */
		geohpp = (GEOH_T **) hdr_addr(h_geo);

		for (band = 0; band < parm.nbands; ++band) {
			if (geohpp[band] != NULL) {


 /* next 2 statements added by Rusty Dodson 8/9/94 to make geodetic
  * extents line up when replicating pixels
  */

		 	  if (parm.force) {  
                               geohpp[band]->bline -= ((geohpp[band]->dline
                               / 2.0) - (geohpp[band]->dline / 2.0 * zline));
                               geohpp[band]->bsamp -= ((geohpp[band]->dsamp
                               / 2.0) - (geohpp[band]->dsamp / 2.0 * zsamp));
			   }

                       	    geoh_dline(geohpp[band]) *= zline;
  			    geoh_dsamp(geohpp[band]) *= zsamp;

			}
		}

		if (geohwrite(parm.o_fd, geohpp) == ERROR) {
			error("can't write GEO header");
		}
	} 
	else {     				/* no geo header */
		if (parm.force) {
		    warn("No geo header.  -f option ignored.");
		}
	}
 /*
  * adjust WIN header
  */
	if (got_hdr(h_win)) {
		int             band;	/* band counter			 */
		WINH_T        **winhpp;	/* -> WIN header array		 */

 /* NOSTRICT */
		winhpp = (WINH_T **) hdr_addr(h_win);

		for (band = 0; band < parm.nbands; ++band) {
			if (winhpp[band] != NULL) {
				winh_dline(winhpp[band]) *= zline;
				winh_dsamp(winhpp[band]) *= zsamp;
			}
		}

		if (winhwrite(parm.o_fd, winhpp) == ERROR) {
			error("can't write WIN header");
		}
	}
}
