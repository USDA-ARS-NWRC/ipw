/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the Computer Systems Laboratory, University of
 * California, Santa Barbara and its contributors'' in the documentation
 * or other materials provided with the distribution and in all
 * advertising materials mentioning features or use of this software.
 *
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

/*
 * demux -- orchestrate image demultiplexing
 */

void
demux(
	int             i_fd,		/* input image file descriptor	 */
	int		nRanges,	/* # of band ranges		 */
	RANGE_T       * ranges,		/* ranges of bands to demux	 */
	int             o_fd)		/* output image file descriptor	 */
{
	BIH_T        ** i_bihpp;	/* -> input BIH			 */
	int             i_nbands;	/* # input bands		 */
	int		i;		/* loop index			 */
	int             o_nbands;	/* # output bands		 */
	int	      * inBands;	/* input band for each out band  */
	int	      * p;		/* working ptr			 */
	BIH_T        ** o_bihpp;	/* -> output BIH		 */
	BIH_T         * i_bihp;		/* -> input band's BIH		 */
	BIH_T         * o_bihp;		/* -> output band's BIH		 */

	int             i_band;		/* current input band #		 */
	int             o_band;		/* current output band #	 */


 /*
  * read BIH
  */
	i_bihpp = bihread(i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

 /*
  * check band selections
  */
	i_nbands = bih_nbands(i_bihpp[0]);

	for (i = 0; i < nRanges; ++i) {
		if (ranges[i].start >= i_nbands) {
			uferr(i_fd);
			error("%d: bad input band number", ranges[i].start);
		} else if (ranges[i].end == -1) {
			ranges[i].end = i_nbands-1;
		} else if (ranges[i].end >= i_nbands) {
			uferr(i_fd);
			error("%d: bad input band number", ranges[i].end);
		}
	}

 /*
  * determine # of output bands, and create a mapping from an output band
  * to its input band.
  */
	o_nbands = 0;
	for (i = 0; i < nRanges; ++i) {
		o_nbands += ranges[i].end - ranges[i].start + 1;
	}
	inBands = (int *) ecalloc(o_nbands, sizeof(int));
	if (inBands == NULL) {
		error("insufficient memory");
	}
	p = inBands;
	for (i = 0; i < nRanges; ++i) {
		int b;

		for (b = ranges[i].start; b <= ranges[i].end; b++) {
			*p++ = b;
		}
	}

 /*
  * make output BIH; first create a BIH for the first output band and use
  * it to create the remaining BIH's.
  */
	o_bihpp = (BIH_T **) ecalloc(o_nbands, sizeof(BIH_T *));
	if (o_bihpp == NULL) {
		error("can't allocate output basic image header");
	}

	i_bihp = i_bihpp[inBands[0]];
	o_bihp = bihmake(bih_nbytes(i_bihp), bih_nbits(i_bihp),
			 dupsv(bih_history(i_bihp)),
			 dupsv(bih_annot(i_bihp)), (BIH_T *) NULL,
			 bih_nlines(i_bihp), bih_nsamps(i_bihp), o_nbands);
	if (o_bihp == NULL) {
		error("can't allocate output basic image header");
	}
	o_bihpp[0] = o_bihp;

	for (i = 1; i < o_nbands; i++) {
		i_bihp = i_bihpp[inBands[i]];
		o_bihp = bihmake(bih_nbytes(i_bihp), bih_nbits(i_bihp),
				 dupsv(bih_history(i_bihp)),
				 dupsv(bih_annot(i_bihp)), o_bihpp[0], 0, 0, 0);
		if (o_bihp == NULL) {
			error("can't allocate output basic image header");
		}
		o_bihpp[i] = o_bihp;
	}

 /*
  * write output BIH
  */
	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}

 /*
  * demultiplex remaining headers
  */
	demuxhdrs(i_fd, i_nbands, o_fd, o_nbands, inBands);

 /*
  * demultiplex image data
  */
	demuximg(i_fd,
		 bih_nlines(i_bihpp[0]), bih_nsamps(i_bihpp[0]), i_nbands,
		 o_fd,
		 o_nbands, inBands);
}
