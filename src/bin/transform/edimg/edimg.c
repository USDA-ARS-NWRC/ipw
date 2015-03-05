#include "ipw.h"

#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * edimg -- replace selected image pixels
 */

void
edimg(
	int             i_fd,		/* input image file descriptor	 */
	int             c_fd,		/* coordinate file descriptor	 */
	bool_t          raw,		/* ? raw replacement values	 */
	double          k,		/* default replacement val	 */
	int             o_fd)		/* output image file descriptor	 */
{
	BIH_T         **bihpp;		/* -> BIH			 */
	BIH_T         **o_bihpp;	/* -> output BIH		 */
	fpixel_t       *buf;		/* -> I/O buffer		 */
	int             eline;		/* next line to edit		 */
	int             line;		/* current input line #		 */
	int             nbands;		/* # image bands		 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / line		 */
	double          repl;		/* replacement value		 */
	int             esamp;		/* next sample to edit		 */

 /*
  * read/write BIH
  */
	bihpp = bihread(i_fd);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	nlines = bih_nlines(bihpp[0]);
	nsamps = bih_nsamps(bihpp[0]);
	nbands = bih_nbands(bihpp[0]);

	o_bihpp = bihdup(bihpp);
	if (o_bihpp == NULL) {
		error("can't duplicate basic image header");
	}
	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * process remaining headers
  */
	if (raw) {
		copyhdrs(i_fd, nbands, o_fd);
	}
	else {
		fphdrs(i_fd, nbands, o_fd);
	}

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}
 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	buf = (fpixel_t *) ecalloc(nsamps * nbands, sizeof(fpixel_t));
	if (buf == NULL) {
		error("can't allocate I/O buffer");
	}
 /*
  * initialize edit coordinates
  */
	getedit(c_fd, nlines, nsamps, k, &eline, &esamp, &repl);
#if DEBUG
	fprintf(stderr, "edit %d,%d <- %g\n", eline, esamp, repl);
#endif
 /*
  * process image lines
  */
	for (line = 0; line < nlines; ++line) {
		if (fpvread(i_fd, buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}
 /*
  * if current line matches edit line then edit it
  */
		while (eline == line) {
			REG_2 int       band;	/* current band #	 */
			REG_1 fpixel_t *bufp;	/* -> current pixel	 */

			bufp = &buf[nbands * esamp];

			for (band = 0; band < nbands; ++band) {
				*bufp++ = (fpixel_t) repl;
			}

			getedit(c_fd, nlines, nsamps, k, &eline, &esamp, &repl);
#if DEBUG
			fprintf(stderr, "edit %d,%d <- %g\n",
				eline, esamp, repl);
#endif
		}

		if (fpvwrite(o_fd, buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}

	if (!ueof(i_fd)) {
		error("input image larger than header indicates");
	}

	SAFE_FREE(buf);
}
