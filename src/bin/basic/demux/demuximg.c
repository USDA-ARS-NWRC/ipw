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
#include "hdrio.h"
#include "pixio.h"

#include "pgm.h"

/*
 * demuximg -- demultiplex image data
 */

void
demuximg(
	int             i_fd,		/* input image file descriptor	 */
	int             nlines,		/* # image lines		 */
	REG_6 int       nsamps,		/* # samples / line		 */
	REG_3 int       i_nbands,	/* input image # bands		 */
	int             o_fd,		/* output image file descriptor	 */
	REG_4 int       o_nbands,	/* output image # bands		 */
	int            *inBands)	/* input[output] band #		 */
{
	pixel_t        *i_buf;		/* -> input buffer		 */
	int             line;		/* current line #		 */
	pixel_t        *o_buf;		/* -> output buffer		 */

 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	i_buf = (pixel_t *) ecalloc(nsamps * i_nbands, sizeof(pixel_t));
	if (i_buf == NULL) {
		error("can't allocate input buffer");
	}
 /* NOSTRICT */
	o_buf = (pixel_t *) ecalloc(nsamps * o_nbands, sizeof(pixel_t));
	if (o_buf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * loop on image lines
  */
	for (line = 0; line < nlines; ++line) {
		int             o_band;	/* current output band #	 */

		if (pvread(i_fd, i_buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}
 /*
  * loop on output bands
  */
		for (o_band = 0; o_band < o_nbands; ++o_band) {
			REG_1 pixel_t  *i_bufp;	/* -> input pixel	 */
			REG_2 pixel_t  *o_bufp;	/* -> output pixel	 */
			REG_5 int       samp;	/* sample #		 */

 /*
  * loop on pixel vectors, copying pixels for current output band
  */
			i_bufp = i_buf + inBands[o_band];
			o_bufp = o_buf + o_band;

			samp = nsamps;
			do {
				*o_bufp = *i_bufp;

				i_bufp += i_nbands;
				o_bufp += o_nbands;
			} while (--samp > 0);
		}

		if (pvwrite(o_fd, o_buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}

	SAFE_FREE(i_buf);
	SAFE_FREE(o_buf);
}
