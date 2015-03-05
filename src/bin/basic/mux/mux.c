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
#include "hdrio.h"
#include "pgm.h"

/*
 * mux -- orchestrate image multiplexing
 */

void
mux(
	int             i_nimgs,	/* # input files		 */
	int            *i_fdp,		/* -> input file descriptors	 */
	int             o_fd)		/* output file descriptor	 */
{
	int             i_img;		/* current input image #	 */
	int             i_band;		/* current input band #	 	 */
	REG_1 BIH_T  ***i_bihv;		/* -> input BIHs		 */
	int            *i_nbandsp;	/* -> input # bands		 */
	int             nlines = 0;	/* # lines		 	 */
	int             nsamps = 0;	/* # samples / line	 	 */
	REG_2 BIH_T   **o_bihpp;	/* -> output BIH		 */
	int             o_band;		/* current output band #	 */
	int             o_nbands;	/* # output bands		 */

 /*
  * allocate array of input BIH pointers
  */
 /* NOSTRICT */
	i_bihv = (BIH_T ***) ecalloc(i_nimgs, sizeof(BIH_T **));
	if (i_bihv == NULL) {
		error("can't allocate array of BIH pointers");
	}

 /*
  * allocate array of input band counts
  */
 /* NOSTRICT */
	i_nbandsp = (int *) ecalloc(i_nimgs, sizeof(int *));
	if (i_nbandsp == NULL) {
		error("can't allocate array of input # bands");
	}

 /*
  * loop through input BIHs
  */
	o_nbands = 0;

	for (i_img = 0; i_img < i_nimgs; ++i_img) {
 /*
  * read BIH
  */
		i_bihv[i_img] = bihread(i_fdp[i_img]);
		if (i_bihv[i_img] == NULL) {
			error("can't read basic image header");
		}

 /*
  * make sure images are same size
  */
		if (i_img == 0) {
			nlines = bih_nlines(i_bihv[0][0]);
			nsamps = bih_nsamps(i_bihv[0][0]);
		}
		else if (bih_nlines(i_bihv[i_img][0]) != nlines ||
			 bih_nsamps(i_bihv[i_img][0]) != nsamps) {
			uferr(i_fdp[i_img]);
			error("image size differs from 1st image");
		}

 /*
  * load band-count vector
  */
		i_nbandsp[i_img] = bih_nbands(i_bihv[i_img][0]);

 /*
  * count total # input bands
  */
		o_nbands += i_nbandsp[i_img];
	}

 /*
  * allocate array of output BIH pointers
  */
 /* NOSTRICT */
	o_bihpp = (BIH_T **) hdralloc(o_nbands, sizeof(BIH_T *),
				      o_fd, BIH_HNAME);
	if (o_bihpp == NULL) {
		error("can't allocate output BIH array");
	}

 /*
  * make output BIH band 0
  */
	o_bihpp[0] = bihmake(bih_nbytes(i_bihv[0][0]), bih_nbits(i_bihv[0][0]),
			     bih_history(i_bihv[0][0]),
			     bih_annot(i_bihv[0][0]),
			     (BIH_T *) NULL, nlines, nsamps, o_nbands);
	if (o_bihpp[0] == NULL) {
		error("can't make BIH for output band 0");
	}

 /*
  * make output BIH bands 1..n-1
  */
	i_band = 1;
	o_band = 1;

	for (i_img = 0; i_img < i_nimgs; ++i_img) {
		for (; i_band < i_nbandsp[i_img]; ++i_band) {
			BIH_T          *i_bihp;

			i_bihp = i_bihv[i_img][i_band];

			o_bihpp[o_band] = bihmake(bih_nbytes(i_bihp),
						  bih_nbits(i_bihp),
						  bih_history(i_bihp),
						  bih_annot(i_bihp),
						  o_bihpp[0],
						  o_nbands, nlines, nsamps);
			if (o_bihpp[o_band] == NULL) {
				error("can't make BIH for output band %d",
				      o_band);
			}

			++o_band;
		}

		i_band = 0;
	}

 /*
  * write output BIH
  */
	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}

 /*
  * multiplex remaining image headers
  */
	muxhdrs(i_nimgs, i_fdp, i_nbandsp, o_fd);

 /*
  * multiplex image data
  */
	muximg(i_nimgs, i_fdp, i_nbandsp, nlines, nsamps, o_fd, o_nbands);

	SAFE_FREE(i_bihv);
	SAFE_FREE(i_nbandsp);
}
