#include <strings.h>

#include "ipw.h"

#include "bih.h"
#include "fpio.h"
#include "hdrio.h"
#include "pixio.h"

#include "pgm.h"

/*
 * convolve -- do image convolution
 */

void
convolve(
	int             i_fd,		/* input image file descriptor	 */
	FILE           *kernelFile,	/* kernel file 			 */
	int             o_fd)		/* output image file descriptor	 */
{
	BIH_T         **bihpp;		/* -> BI header array		 */
	BIH_T        **o_bihpp;		/* -> output BI header array	 */
	fpixel_t      **kernel;		/* -> kernel array		 */
	REG_1 fpixel_t ***kmap;		/* -> kernel fpixel[pixel] maps	 */
	REG_2 pixel_t **i_bufs;		/* -> input line buffers	 */
	int             i_line;		/* current input line #		 */
	int             nbands;		/* # image bands		 */
	int             ncols;		/* # kernel columns		 */
	int             nlines;		/* # image lines		 */
	int             nrows;		/* # kernel rows		 */
	int             nsamps;		/* # samples / image line	 */
	fpixel_t       *o_buf;		/* -> output line buffer	 */
	int             o_line;		/* current output line #	 */
	int             rslop;		/* # unconvolvable lines	 */

 /*
  * read kernel
  */
	kernel = getkernel(kernelFile, &nrows, &ncols);
#ifdef DEBUG
	putkernel(kernel, nrows, ncols);
#endif
 /*
  * read BIH
  */
	bihpp = bihread(i_fd);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	nlines = bih_nlines(bihpp[0]);
	nsamps = bih_nsamps(bihpp[0]);
	nbands = bih_nbands(bihpp[0]);
 /*
  * image must be at least as big as the kernel
  */
	if (nrows > nlines || ncols > nsamps) {
		error("%dx%d kernel is bigger than %dx%d image",
		      nrows, ncols, nlines, nsamps);
	}
 /*
  * NB: this should be relaxed someday ...
  */
	if (nbands > 1) {
		uferr(i_fd);
		error("sorry, only single-band input images accepted");
	}
 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	i_bufs = (pixel_t **) allocnd(sizeof(pixel_t), 2, nrows, nsamps);
	if (i_bufs == NULL) {
		error("can't allocate image input buffers");
	}
 /* NOSTRICT */
	o_buf = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	if (o_buf == NULL) {
		error("can't allocate image output buffer");
	}
 /*
  * write BIH
  */
	o_bihpp = bihdup(bihpp);
	if (o_bihpp == NULL) {
		error("can't duplicate basic image header");
	}
	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * copy remaining headers; initialize fpio
  */
	fphdrs(i_fd, nbands, o_fd);

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}
 /*
  * initialize kernel (weight*fpixel)[pixel] maps
  */
	kmap = do_kmap(kernel, nrows, ncols,
		       fpmap(i_fd)[0], fpmaplen(i_fd)[0]);
 /*
  * rslop is number of unconvolvable lines at {beginning,end} of image
  */
	rslop = nrows / 2;
 /*
  * read first kernel's worth of lines
  */
	for (i_line = 0; i_line < nrows; ++i_line) {
		if (pvread(i_fd, i_bufs[i_line], nsamps) != nsamps) {
			error("image read failed, line %d", i_line);
		}
	}
 /*
  * write initial unconvolved "slop" lines
  */
	for (o_line = 0; o_line < rslop; ++o_line) {
		if (pvwrite(o_fd, i_bufs[o_line], nsamps) != nsamps) {
			error("image write failed, line %d", o_line);
		}
	}
 /*
  * loop through convolvable lines
  */
	for (;;) {
		REG_3 int       row;	/* current kernel row		 */

		row = nrows - 1;
		do {
			if (kmap[row] != NULL) {
				conv1d(i_bufs[row], nsamps,
				       kmap[row], ncols,
				       o_buf);
			}
		} while (--row >= 0);

		if (fpvwrite(o_fd, o_buf, nsamps) == ERROR) {
			error("image write failed, line %d", o_line);
		}

		if (i_line >= nlines) {
			break;
		}

		if (pvread(i_fd, i_bufs[0], nsamps) != nsamps) {
			error("image read failed, line %d", i_line);
		}

		bufcycle(i_bufs, nrows);
 /* NOSTRICT */
		bzero((char *) o_buf, nsamps * sizeof(fpixel_t));

		++i_line;
		++o_line;
	}
 /*
  * write trailing unconvolved "slop" lines
  */
	while (++rslop < nrows) {
		if (pvwrite(o_fd, i_bufs[rslop], nsamps) != nsamps) {
			error("image write failed, line %d", o_line);
		}

		++o_line;
	}
 /*
  * clean up
  */
	SAFE_FREE(o_buf);
	SAFE_FREE(i_bufs);
	SAFE_FREE(kernel);
	/*
	 * We should free the allocated part of kmap, but it's such a mess
	 * that we can't free it since there are multiple pointers to allocated
	 * areas, so we'd end up freeing them multiple times.
	 */
	SAFE_FREE(kmap);
}
