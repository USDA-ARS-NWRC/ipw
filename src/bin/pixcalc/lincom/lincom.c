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

/*
 * lincom -- linear combination of bands
 *
 * write output to scratch file, keeping track of min and max
 */

#include "ipw.h"
#include "bih.h"

#include "pgm.h"

void
lincom(void)
{
	REG_1 int       ib;		/* input band index	 */
	REG_2 fpixel_t **btbl;		/* band tbl for fp conv	 */
	REG_3 pixel_t  *ip;		/* -> ibuf		 */
	REG_4 int       i_bands;	/* # input bands	 */
	REG_5 fpixel_t *op;		/* -> obuf		 */
	REG_6 pixel_t  *ihold;		/* -> ibuf		 */
	fpixel_t       *holdmm;		/* -> temp min/max	 */
	fpixel_t       *minmax;		/* -> min/max each line	 */
	fpixel_t       *obuf;		/* -> output buffer	 */
	fpixel_t       *pmax;		/* -> maxima		 */
	fpixel_t       *pmin;		/* -> minima		 */
	int             fdt;		/* temp file descriptor	 */
	int             line;		/* line index		 */
	int             nbytes;		/* # bytes to write	 */
	int             nlines;		/* # lines in image	 */
	int             nsamps;		/* # pixels per line	 */
	int             nwrite;		/* # pixels written	 */
	int             o_bands;	/* # output bands	 */
	int             ob;		/* output band index	 */
	int             pixel;		/* pixel index in line	 */
	pixel_t        *ibuf;		/* -> input buffer	 */

	i_bands = hnbands(parm.i_fd);
	o_bands = hnbands(parm.o_fd);
	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	ibuf = (pixel_t *) ecalloc(i_bands * nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}
 /* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(o_bands * nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * create temporary file
  */
	if ((parm.tmpnam = mktemplate(IPW_NAME)) == NULL) {
		error("can't create temporary file name");
	}
	if ((fdt = uwopen_temp(parm.tmpnam)) == ERROR) {
		error("can't create temporary file");
	}
 /*
  * vector to hold min and max of all lines
  */
 /* NOSTRICT */
	minmax = (fpixel_t *) ecalloc(nlines * 2 * o_bands, sizeof(fpixel_t));
	if (minmax == NULL) {
		error("can't store min and max of each line");
	}
 /*
  * vector to hold min and max of one line
  */
 /* NOSTRICT */
	holdmm = (fpixel_t *) ecalloc(2 * o_bands, sizeof(fpixel_t));
	if (holdmm == NULL) {
		error("can't store min and max of each line");
	}
 /*
  * pointers to vectors of minima and maxima
  */
	pmin = minmax;
	pmax = &minmax[nlines * o_bands];
 /*
  * read input and create temporary output image; calculate min and max
  * of each line as we go along
  */
	for (line = 0; line < nlines; ++line) {
		nwrite = pvread(parm.i_fd, ibuf, nsamps);
		if (nwrite < nsamps) {
			warn("premature end of image, line %d", line);
			if (nwrite <= 0)
				break;
		}
		ihold = ibuf;
		op = obuf;
		for (pixel = nwrite; --pixel >= 0;) {
			for (ob = 0; ob < o_bands; ++ob) {
				*op = 0;
				btbl = parm.ftbl[ob];
				ip = ihold;
				for (ib = 0; ib < i_bands; ++ib) {
					*op += btbl[ib][*ip++];
				}
				++op;
			}
			ihold += i_bands;
		}
 /*
  * output line - also calculate min and max
  */
		nbytes = nwrite * o_bands * sizeof(fpixel_t);
 /* NOSTRICT */
		if (uwrite(fdt, (addr_t) obuf, nbytes) != nbytes) {
			error("temp file write error, line %d", line);
		}
		if (mnxfp(obuf, nwrite, o_bands, holdmm) == ERROR) {
			error("can't calculate min/max, line %d", line);
		}
 /*
  * store min and max in global array
  */
		for (ob = 0; ob < o_bands; ++ob) {
			pmin[ob] = holdmm[2 * ob];
			pmax[ob] = holdmm[2 * ob + 1];
		}
		pmin += o_bands;
		pmax += o_bands;
	}
 /*
  * calculate global min/max
  */
	if (line < nlines) {		/* allow for premature end of
					 * image */
		nlines = line;
	}
	if (mnxfp(minmax, 2 * nlines, o_bands, holdmm) == ERROR) {
		error("can't calculate global min/max");
	}
 /*
  * store values in external parameter structure
  */
	parm.mmval = holdmm;
 /*
  * resolve problem if min equal to maximum
  */
	if (parm.mmval[0] == parm.mmval[1]) {
		parm.mmval[1] += 1;
	}
 /*
  * clean up
  */
	(void) uclose(fdt);
 /* NOSTRICT */
	free((addr_t) ibuf);
 /* NOSTRICT */
	free((addr_t) obuf);
 /* NOSTRICT */
	free((addr_t) minmax);
}
