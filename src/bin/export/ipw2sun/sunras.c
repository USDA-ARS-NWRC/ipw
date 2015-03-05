#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * sunras -- convert IPW image to Sun bitmap
 */

void
sunras(
	int             i_fd,		/* input image file descriptor	 */
	int             o_fd)		/* output image file descriptor	 */
{
	BIH_T         **bihpp;		/* -> input BIH			 */
	char           *i_buf;		/* -> input buffer		 */
	int             line;		/* current line #		 */
	int             nbits;		/* # bits / pixel		 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / line	 	 */
	int             nbytes_line;	/* # bytes / output line	 */
	char           *o_buf;		/* -> output buffer		 */

 /*
  * read and check BIH
  */
	bihpp = bihread(i_fd);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	nlines = bih_nlines(bihpp[0]);
	nsamps = bih_nsamps(bihpp[0]);
	nbits = bih_nbits(bihpp[0]);

	if (bih_nbands(bihpp[0]) != 1) {
		error("input image has %d bands (only 1 allowed)",
		      bih_nbands(bihpp[0]));
	}

	if (bih_nbytes(bihpp[0]) != 1) {
		error("input image has %d-byte pixels (only 1 allowed)",
		      bih_nbytes(bihpp[0]));
	}

	if (!(nbits == 1 || nbits == 8)) {
		error("input image has %d-bit pixels (only 1 or 8 allowed)",
		      nbits);
	}
 /*
  * skip remaining headers
  */
	skiphdrs(i_fd);
 /*
  * create and write Sun rasterfile header
  */
	nbytes_line = sunhdr(nlines, nsamps, nbits, o_fd);
 /*
  * allocate output buffer
  */
	o_buf = ecalloc(nbytes_line, 1);
	if (o_buf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * allocate input buffer: if 8-bit pixels then just use output buffer
  */
	if (nbits == 8) {
		i_buf = o_buf;
	}
	else {
		i_buf = ecalloc(nsamps, 1);
		if (i_buf == NULL) {
			error("can't allocate input buffer");
		}
	}
 /*
  * loop through input lines
  */
	for (line = 0; line < nlines; ++line) {
		if (uread(i_fd, i_buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}

		if (nbits == 1) {
			bitmap(i_buf, o_buf, nsamps);
		}

		if (uwrite(o_fd, o_buf, nbytes_line) != nbytes_line) {
			error("image write failed, line %d", line);
		}
	}

	SAFE_FREE(o_buf);
	if (nbits != 8) {
		SAFE_FREE(i_buf);
	}
}
