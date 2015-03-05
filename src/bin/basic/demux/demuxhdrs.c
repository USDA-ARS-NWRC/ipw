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
#include "pgm.h"


/*
 * demuxhdrs -- demultiplex image headers
 */

void
demuxhdrs(
	int             i_fd,		/* input image file descriptor	 */
	int             i_nbands,	/* # input bands		 */
	int             o_fd,		/* output image file descriptor	 */
	int             o_nbands,	/* # output bands		 */
	int           * inBands)	/* input[output] band #		 */
{
	int           * outBands;	/* out bands for each input band */
	int             i_band;		/* current input band #		 */
	int             o_band;		/* current output band #	 */
	char          * hname;		/* current header name		 */

 /*
  * construct a mapping from an input band to its corresponding output band
  */
	outBands = (int *) ecalloc(i_nbands, sizeof(int));
	if (outBands == NULL) {
	    error("insufficient memory");
	}
	for (i_band = 0; i_band < i_nbands; i_band++) {
	    outBands[i_band] = -1;
	}
	for (o_band = 0; o_band < o_nbands; o_band++) {
	    if (outBands[inBands[o_band]] == -1) {
		outBands[inBands[o_band]] = o_band;
	    } else {
		error("input band %d specified more than once",
			inBands[o_band]);
	    }
	}

 /*
  * process headers
  */
	while ((hname = hrname(i_fd)) != NULL && strdiff(hname, BOIMAGE)) {
	    i_band = hrband(i_fd);

	    if (i_band < 0 || i_band >= i_nbands) {
		error("\"%s\" header, band %d: no such band", hname, i_band);
	    }

 /*
  * if not extracting this band then skip this header
  */
	    o_band = outBands[i_band];
	    if (o_band == -1) {
		if (hrskip(i_fd) == ERROR) {
		    error("can't skip %s header", hname);
		}
	    }

 /*
  * if extracting this band then copy this header, with new band number
  */
	    else {
		if (hwprmb(o_fd, hname, o_band, hrvers(i_fd)) == ERROR) {
		    error("\"%s\" header, band %d: can't write preamble", hname,
			 o_band);
		}

		if (hpass(i_fd, o_fd) == ERROR) {
		    error("\"%s\" header, band %d: can't copy", hname, o_band);
		}
	    }
	}  /* while */

	if (hname == NULL) {
	    error("header read error");
	}

	if (boimage(o_fd) == ERROR) {
	    error("can't terminate header output");
	}

	SAFE_FREE(outBands);
}
