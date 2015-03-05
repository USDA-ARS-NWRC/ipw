#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "winh.h"

#include "pgm.h"

void
mkwinh(
	int             fdi,		/* intput file descriptor	 */
	double          bline,		/* begin line #			 */
	double          bsamp,		/* begin sample #		 */
	double          dline,		/* line increment 		 */
	double          dsamp,		/* sample increment		 */
	bool_t          force,          /* don't copy input image        */
	int             fdo)		/* output file descriptor	 */
{
	static GETHDR_T h_winh = {WINH_HNAME, NULL};
	static GETHDR_T *hv[] = {&h_winh, NULL};

	int             band;		/* loop counter			 */
	BIH_T         **bihpp;		/* -> basic image header	 */
	int             nbands;		/* image # bands		 */
	WINH_T         *winhp;		/* -> WIN header	 	 */
	WINH_T        **winhpp;		/* -> WIN header pointer array	 */
	bool_t          madehdr;	/* did we allocate header space? */

 /*
  * read and write BIH
  */
	bihpp = bihread(fdi);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	nbands = bih_nbands(bihpp[0]);

	if (bihwrite(fdo, bihpp) != OK) {
		error("can't write basic image header");
	}
 /*
  * make window header
  */
	winhp = winhmake(bline, bsamp, dline, dsamp);
	if (winhp == NULL) {
		error("can't make window header");
	}
 /*
  * copy remaining headers: shortstop any window headers
  */
	gethdrs(fdi, hv, nbands, fdo);
 /*
  * if existing window header then use it
  */
	madehdr = FALSE;

	if (got_hdr(h_winh)) {
 /* NOSTRICT */
		winhpp = (WINH_T **) hdr_addr(h_winh);
	}
 /*
  * if no existing window header then allocate pointer array
  */
	else {
 /* NOSTRICT */
		winhpp = (WINH_T **) hdralloc(nbands, sizeof(WINH_T *),
					      ERROR, WINH_HNAME);
		if (winhpp == NULL) {
			error("can't allocate window header pointer array");
		}
		madehdr = TRUE;
	}
 /*
  * link window header into pointer array
  */
	for (band = 0; band < nbands; ++band) {
		winhpp[band] = winhp;
	}
 /*
  * write window header
  */
	if (winhwrite(fdo, winhpp) != OK) {
		error("can't write window header");
	}

	if (boimage(fdo) != OK) {
		error("can't terminate header output");
	}
 /*
  * copy image
  */
	
	if (!force) {
		if (imgcopy(fdi, fdo) == ERROR) {
			error("image copy failed");
		}
	}

	if (madehdr) {
		SAFE_FREE(winhpp);
	}
}
