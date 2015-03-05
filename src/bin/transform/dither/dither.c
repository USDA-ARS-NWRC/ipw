#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "pgm.h"

/*
 * dither -- orchestrate image dithering
 */

void
dither(
	int             i_fd,		/* input image file descriptor	 */
	int             rank,		/* dither matrix rank		 */
	int             o_fd)		/* output image file descriptor	 */
{
	int             band;		/* current band #		 */
	pixel_t      ***dm;		/* -> dither matrix		 */
	BIH_T         **i_bihpp;	/* -> input BIH			 */
	int             nbands;		/* # image bands		 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / image line	 */
	BIH_T         **o_bihpp;	/* -> output BIH		 */

 /*
  * read input BIH
  */
	i_bihpp = bihread(i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	nlines = bih_nlines(i_bihpp[0]);
	nsamps = bih_nsamps(i_bihpp[0]);
	nbands = bih_nbands(i_bihpp[0]);
 /*
  * create and write output BIH (same as input except 1 byte, 1 bit per
  * pixel)
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't allocate output BIH");
	}

	for (band = 0; band < nbands; ++band) {
		bih_nbytes(o_bihpp[band]) = 1;
		bih_nbits(o_bihpp[band]) = 1;
	}

	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * copy remaining headers
  */
	copyhdrs(i_fd, nbands, o_fd);

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}
 /*
  * construct dither matrix
  */
	dm = mk_dm(rank, nbands, i_bihpp);
 /*
  * dither the image
  */
	dithimg(i_fd, nlines, nsamps, nbands, dm, rank, o_fd);
 /*
  * clean up
  */
	SAFE_FREE(dm);
}
