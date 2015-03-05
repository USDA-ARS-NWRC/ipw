#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * pspic -- orchestrate conversion of image to PostScript
 */

void
pspic(
	int             fdi,		/* input image file descriptor	 */
	double          h_page,		/* PostScript page height	 */
	double          w_page,		/* PostScript page width	 */
	double          h_img,		/* PostScript image height	 */
	double          w_img,		/* PostScript image width	 */
	bool_t          rotate)		/* ? rotate output image 90 deg	 */
{
	BIH_T         **bihpp;		/* -> input BIH			 */

 /*
  * read and verify BIH
  */
	bihpp = bihread(fdi);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	if (bih_nbands(bihpp[0]) != 1) {
		error("input image must have only 1 band");
	}

	if (bih_nbytes(bihpp[0]) != 1) {
		error("input image must have only 1 byte per pixel");
	}
 /*
  * skip remaining headers
  */
	skiphdrs(fdi);
 /*
  * convert input image
  */
	pspicx(fdi, bih_nlines(bihpp[0]), bih_nsamps(bihpp[0]),
	       bih_nbits(bihpp[0]), h_page, w_page, h_img, w_img, rotate);
}
