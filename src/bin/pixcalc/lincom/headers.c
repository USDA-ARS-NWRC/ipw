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
#include "gethdrs.h"
#include "lqh.h"

#include "pgm.h"

/*
** headers -- process image headers
*/

extern void     head1(void);
extern void     head2(void);

void
headers(
	bool_t          already)
{
	if (!already) {
		head1();
	}
	else {
		head2();
	}
}

/*
** first pass: read all headers; write BIH; read/convert LQH
*/

void
head1(void)
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	int             band;		/* band index			 */
	int             nbands;		/* # input bands		 */
	int             obands;		/* # output bands		 */

 /* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

 /*
  * read input BIH
  */
	if ((i_bih = bihread(parm.i_fd)) == NULL) {
		error("can't read input BIH");
	}
 /*
  * determine nbits if not set from command line
  */
	assert(parm.nbits >= 0);
	nbands = bih_nbands(i_bih[0]);
	if (parm.nbits == 0) {
		for (band = nbands; --band >= 0;) {
			if (parm.nbits < bih_nbits(i_bih[band]))
				parm.nbits = bih_nbits(i_bih[band]);
		}
	}
 /*
  * determine number of output bands
  */
	if (parm.ncoef == 0 || parm.ncoef == 1 || parm.ncoef == nbands) {
		obands = 1;
	}
	else if (parm.ncoef % nbands == 0) {
		obands = parm.ncoef / nbands;
	}
	else {
		error("# coefficients specified = %d; %s",
		      parm.ncoef, "must be zero, 1, or modulo nbands");
	}
 /*
  * create and write output BIH
  */
 /* NOSTRICT */
	o_bih = (BIH_T **) hdralloc(obands, sizeof(BIH_T *),
				    parm.o_fd, BIH_HNAME);
	assert(o_bih != NULL);
	for (band = 0; band < obands; ++band) {
		o_bih[band] = bihmake(0, parm.nbits,
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
				      o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
				      obands);
		if (o_bih[band] == NULL) {
			error("can't make output BIH");
		}
	}
	if (bihwrite(parm.o_fd, o_bih) == ERROR) {
		error("can't write BIH");
	}
 /*
  * read LQH, other headers get passed thru
  * K. Longley -- use obands instead of nbands here
  */
	gethdrs(parm.i_fd, request, obands, parm.o_fd);
}

/*
** create and write LQH
*/

void
head2(void)
{
	LQH_T         **o_lqh;		/* output LQH			 */
	fpixel_t       *fval;		/* min, max of output LQH	 */
	int             band;		/* band index			 */
	int             nbits = 0;	/* # bits / pixel in output	 */
	int             obands;		/* # output bands		 */
	pixel_t        *ival;		/* min,max pixel values		 */
	pixel_t        *ivalp;		/* min,max pixel values pointer	 */

	obands = hnbands(parm.o_fd);
 /* NOSTRICT */
	ival = (pixel_t *) ecalloc(obands * 2, sizeof(pixel_t));
	assert(ival != NULL);
 /*
  * set pixel_t mapping to 0 and 2^nbits - 1
  */
	for (band = 0; band < obands; ++band) {
		nbits = hnbits(parm.o_fd, band);
		assert(nbits > 0);
		ival[2 * band] = 0;
		ival[2 * band + 1] = ipow2(nbits) - 1;
	}
 /*
  * create output LQH
  */
 /* NOSTRICT */
	o_lqh = (LQH_T **) hdralloc(obands, sizeof(LQH_T *),
				    parm.o_fd, LQH_HNAME);
	assert(o_lqh != NULL);
	fval = parm.mmval;
	ivalp = ival;
	for (band = 0; band < obands; ++band) {
		o_lqh[band] = lqhmake(nbits, 2, ivalp, fval,
				      (char *) NULL, (char *) NULL);
		if (o_lqh[band] == NULL) {
			error("can't create output LQH");
		}
		ivalp += 2;
		fval += 2;
	}
	if (lqhwrite(parm.o_fd, o_lqh) == ERROR) {
		error("can't write output LQH");
	}
 /*
  * get ready for rest of output
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't initialize image output");
	}
	SAFE_FREE(ival);
}
