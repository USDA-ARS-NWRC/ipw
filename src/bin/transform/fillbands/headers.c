 /*
  * read and write headers
  */
#include "ipw.h"

#include "gethdrs.h"

#include "bih.h"
#include "lqh.h"
#include "orh.h"
#include "geoh.h"
#include "fpio.h"
#include "pgm.h"


void
headers(
	int             fdi,		/* input image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	int		outbands)	/* # bands in output		 */
{
	BIH_T         **i_bih;		/* -> input BIH			 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	LQH_T         **i_lqh;		/* -> input LQH			 */
	LQH_T         **o_lqh;		/* -> output LQH		 */
	GEOH_T        **i_geoh;         /* -> input geodetic header     */
	GEOH_T        **o_geoh;         /* -> output geodetic header     */
	LQH_T         **newlqh();	/* function to write output lqh	 */
	int             j;		/* counter			 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
        static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_lqh, &h_geo, 0};

 /*
  * read BIH
  */
	if ((i_bih = bihread(fdi)) == NULL) {
		error("can't read input image BIH");
	}

	if (hnbands(fdi) != 2) {
		error("input image must have exactly 2 bands.");
	}
 /*
  * create output BIH, # bytes will be taken from input image
  */
	o_bih = (BIH_T **) hdralloc(outbands, sizeof(BIH_T *), fdo, BIH_HNAME);
	for (j = 0; j < outbands; ++j) {
		o_bih[j] = bihmake(0, bih_nbits(i_bih[0]),
				   bih_history(i_bih[0]), bih_annot(i_bih[0]),
				   o_bih[0],
				   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
				   outbands);
	}
 /*
  * write output BIH
  */
	if (bihwrite(fdo, o_bih) == ERROR) {
		error("Can't write output BIH");
	}
 /*
  * ingest LQH, GEOH, other headers get
  * passed thru
  */
	gethdrs(fdi, request, outbands, fdo);

	if (hdr_addr(h_lqh) == NULL)
		error("input image must have an LQH");
	else (i_lqh = (LQH_T **) hdr_addr(h_lqh));
 /*
  * make the output LQH
  */
	if (lqh_nbkpts(i_lqh[0]) != 2 || lqh_nbkpts(i_lqh[1]) != 2) {
		error("both input bands must have exactly 2 lq breakpoints.");
	}

	o_lqh = newlqh(fdo, i_lqh );
	if (o_lqh == NULL) {
		error("Can't create new LQH");
	}
 /*
  * write output LQH
  */
	if (lqhwrite(fdo, o_lqh) == ERROR) {
		error("Can't write output LQH");
	}
 /*
  * if input GEOH exists, copy output hdrs from input band 0 
  */
        if ( (i_geoh = (GEOH_T **) hdr_addr(h_geo)) != NULL) {
	    o_geoh = (GEOH_T **) hdralloc(outbands, sizeof(GEOH_T *),
	  	fdo, GEOH_HNAME);
	    for (j = 0; j < outbands; ++j) { 
		o_geoh[j] = i_geoh[0]; 
	    }
            if (geohwrite(fdo, o_geoh) == ERROR) { 
		error("Can't write GEOH"); 
	    }
	}
 /*
  * get ready for rest of output
  */
	if (boimage(fdo) == ERROR) {
		error("Can't terminate header output");
	}

}
