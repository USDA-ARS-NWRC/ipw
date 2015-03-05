#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "sunh.h"

#include "pgm.h"

void
mksunh(
	int             fdi,		/* input file descriptor	 */
	double		azimuth,	/* solar azimuth		 */
	double		cos_zen,	/* cosine of solar zenith	 */
	bool_t          force,          /* don't copy input image        */
	int             fdo)		/* output file descriptor	 */
{
	static GETHDR_T h_sunh = {SUNH_HNAME, NULL};
	static GETHDR_T *hv[] = {&h_sunh, NULL};

	int             band;		/* loop counter			 */
	BIH_T         **bihpp;		/* -> basic image header	 */
	int             nbands;		/* image # bands		 */
	SUNH_T         *sunhp;		/* -> SUN header	 	 */
	SUNH_T        **sunhpp;		/* -> SUN header pointer array	 */
	bool_t		madehdr;	/* did we allocate header space? */

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
  * make sun header
  */
	sunhp = sunhmake(cos_zen, azimuth);
	if (sunhp == NULL) {
		error("can't make sun header");
	}
 /*
  * copy remaining headers: shortstop any sun headers
  */
	gethdrs(fdi, hv, nbands, fdo);
 /*
  * if existing sun header then use it
  */
	madehdr = FALSE;

	if (got_hdr(h_sunh)) {
 /* NOSTRICT */
		sunhpp = (SUNH_T **) hdr_addr(h_sunh);
	}
 /*
  * if no existing sun header then allocate pointer array
  */
	else {
 /* NOSTRICT */
		sunhpp = (SUNH_T **) hdralloc(nbands, sizeof(SUNH_T *),
					      ERROR, SUNH_HNAME);
		if (sunhpp == NULL) {
			error("can't allocate sun header pointer array");
		}
		madehdr = TRUE;
	}
 /*
  * link sun header into pointer array
  */
	for (band = 0; band < nbands; ++band) {
		sunhpp[band] = sunhp;
	}
 /*
  * write sun header
  */
	if (sunhwrite(fdo, sunhpp) != OK) {
		error("can't write sun header");
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
		SAFE_FREE(sunhpp);
	}
}
