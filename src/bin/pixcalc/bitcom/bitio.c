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

/* LINTLIBRARY */

#include "ipw.h"
#include "pixio.h"

#include "pgm.h"

/*
 * bitio -- handle I/O for bitwise band combination
 */

void
bitio(
	int             i_fd,		/* input image file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / line		 */
	int             nbands,		/* # bands / sample		 */
	bool_t          mflag,		/* ? last band is a mask	 */
	void            (*op) (pixel_t *buf, int nsamps, int nbands),
					/* -> bitwise operator function	 */
	int             o_fd)		/* output image file descriptor	 */
{
	pixel_t        *io_buf;		/* -> image buffer		 */
	int             line;		/* current line #		 */

 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	io_buf = (pixel_t *) ecalloc(nsamps * nbands, sizeof(pixel_t));
	if (io_buf == NULL) {
		error("can't allocate image I/O buffer");
	}
 /*
  * loop through image lines
  */
	for (line = 0; line < nlines; ++line) {
 /*
  * read image line
  */
		if (pvread(i_fd, io_buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}
 /*
  * if mask flag set then expand pixels in last band
  */
		if (mflag) {
			m_expand(io_buf, nsamps, nbands);
		}
 /*
  * do bitwise combination
  */
		(*op) (io_buf, nsamps, nbands);
 /*
  * write image line
  */
		if (pvwrite(o_fd, io_buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}

	SAFE_FREE(io_buf);
}
