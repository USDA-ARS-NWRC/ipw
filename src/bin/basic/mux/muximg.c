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
#include "pixio.h"
#include "pgm.h"

/*
 * muximg -- multiplex image data
 */

void
muximg(
	int             i_n,		/* # input files		 */
	int            *i_fdp,		/* -> input file descriptors	 */
	int            *i_nbandsp,	/* -> input # bands		 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / line		 */
	int             o_fd,		/* output file descriptor	 */
	int             o_nbands)	/* # output bands		 */
{
	pixel_t        *i_buf;		/* -> input buffer		 */
	int             i;		/* current input file image #	 */
	int             i_maxnbands;	/* max # bands / input image	 */
	int             line;		/* current line #		 */
	pixel_t        *o_buf;		/* -> output buffer		 */
	int            *o_skip;		/* o_buf increment [i]		 */
	pixel_t       **o_start;	/* o_buf start addr [i]		 */

 /*
  * allocate I/O buffers
  */
	i_maxnbands = i_nbandsp[0];
	for (i = 1; i < i_n; ++i) {
		if (i_maxnbands < i_nbandsp[i]) {
			i_maxnbands = i_nbandsp[i];
		}
	}

 /* NOSTRICT */
	i_buf = (pixel_t *) ecalloc(nsamps * i_maxnbands, sizeof(pixel_t));
	if (i_buf == NULL) {
		error("can't allocate input buffer");
	}

 /* NOSTRICT */
	o_buf = (pixel_t *) ecalloc(nsamps * o_nbands, sizeof(pixel_t));
	if (o_buf == NULL) {
		error("can't allocate output buffer");
	}

 /*
  * allocate and initialize o_start vector: o_start[i] == initial address, in
  * output buffer, of pixels from input image i
  */
 /* NOSTRICT */
	o_start = (pixel_t **) ecalloc(i_n, sizeof(pixel_t *));
	if (o_start == NULL) {
		error("can't allocate output buffer address array");
	}

	o_start[0] = &o_buf[0];
	for (i = 1; i < i_n; ++i) {
		o_start[i] = o_start[i - 1] + i_nbandsp[i - 1];
	}

 /*
  * allocate and initialize o_skip vector: o_skip[i] == # pixels, in o_buf,
  * to skip between pixels from input image i
  */
 /* NOSTRICT */
	o_skip = (int *) ecalloc(i_n, sizeof(int));
	if (o_skip == NULL) {
		error("can't allocate output buffer skip vector");
	}

	for (i = 0; i < i_n; ++i) {
		o_skip[i] = o_nbands - i_nbandsp[i];
	}

 /*
  * I/O loop.  We read a line from each input image and shuffle it into the
  * output line.  Once this has been done for each input image, we write the
  * output line.  This procedure is repeated until we run out of input.
  */
	for (line = 0; line < nlines; ++line) {
		for (i = 0; i < i_n; ++i) {
			REG_2 pixel_t  *i_bufp;
			REG_6 int       nbands;
			REG_1 pixel_t  *o_bufp;
			REG_4 int       skip;
			REG_5 int       tmp_nsamps;

			if (pvread(i_fdp[i], i_buf, nsamps) != nsamps) {
				error("image read failed, line %d", line);
			}

			i_bufp = i_buf;
			o_bufp = o_start[i];
			skip = o_skip[i];
			nbands = i_nbandsp[i];

			tmp_nsamps = nsamps;
			do {
				REG_3 int       tmp_nbands;

				tmp_nbands = nbands;
				do {
					*o_bufp++ = *i_bufp++;
				} while (--tmp_nbands > 0);

				o_bufp += skip;
			} while (--tmp_nsamps > 0);
		}

		if (pvwrite(o_fd, o_buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}
	SAFE_FREE(i_buf);
	SAFE_FREE(o_buf);
	SAFE_FREE(o_start);
	SAFE_FREE(o_skip);
}
