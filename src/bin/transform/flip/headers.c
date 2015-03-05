#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "geoh.h"
#include "orh.h"
#include "winh.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(void)
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */
	ORH_T         **i_orhpp;	/* -> input ORH array		 */
	ORH_T         **o_orhpp;	/* -> output ORH array		 */
	GEOH_T        **i_geohpp;	/* -> input GEOH array		 */
	GEOH_T        **o_geohpp;	/* -> output GEOH array		 */
	WINH_T        **i_winhpp;	/* -> input WINH array		 */
	WINH_T        **o_winhpp;	/* -> output WINH array		 */

 /* NOSTRICT */
	static GETHDR_T h_orh = {ORH_HNAME, (ingest_t) orhread};
 /* NOSTRICT */
	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
 /* NOSTRICT */
	static GETHDR_T h_winh = {WINH_HNAME, (ingest_t) winhread};
	static GETHDR_T *request[] = {&h_orh, &h_geoh, &h_winh, 0};

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}
 /*
  * reset o_byteorder from hostorder() to byte order of parm.i_fd
  * because this program will bypass the pixio layer
  */
	o_byteorder = hbyteorder(parm.i_fd);
 /*
  * create and write BIH
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}

	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * process remaining headers -- all but ORH, GEOH, & WINH get passed
  * thru
  */
	gethdrs(parm.i_fd, request, hnbands(parm.o_fd), parm.o_fd);
 /*
  * fix orientation header, if we got one, otherwise create new ORH
  */
 /* NOSTRICT */
	i_orhpp = (ORH_T **) hdr_addr(h_orh);
	o_orhpp = fixorh(i_orhpp);
 /*
  * fix window header, only if we got one
  */
 /* NOSTRICT */
	i_winhpp = (WINH_T **) hdr_addr(h_winh);
	o_winhpp = fixwinh(i_winhpp);
 /*
  * fix geodetic header, only if we got one
  */
 /* NOSTRICT */
	i_geohpp = (GEOH_T **) hdr_addr(h_geoh);
	o_geohpp = fixgeoh(i_geohpp);
 /*
  * write new headers; could be NULL either if not needed (ORH) or if
  * not input header (WINH or GEOH)
  */
	if (o_orhpp != NULL) {
		if (orhwrite(parm.o_fd, o_orhpp) == ERROR)
			error("can't write new ORH");
		(void) orhfree(o_orhpp, hnbands(parm.o_fd));
	}
	if (o_winhpp != NULL) {
		if (winhwrite(parm.o_fd, o_winhpp) == ERROR)
			error("can't write new WINH");
		(void) winhfree(o_winhpp, hnbands(parm.o_fd));
	}
	if (o_geohpp != NULL) {
		if (geohwrite(parm.o_fd, o_geohpp) == ERROR)
			error("can't write new GEOH");
		(void) geohfree(o_geohpp, hnbands(parm.o_fd));
	}
 /*
  * done
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}

}
