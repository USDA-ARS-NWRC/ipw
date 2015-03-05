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

void
mkbih(
	int             fdi,		/* input file descriptor	 */
	int             nbands,		/* image # bands		 */
	bool_t          repeat,		/* ? repeat band[0] initializers */
	bool_t		annotf,		/* ? multiple band annotation    */
	int             nlines,		/* image # lines		 */
	int             nsamps,		/* # samples / line		 */
	int            *nbytes,		/* # bytes / pixel		 */
	int            *nbits,		/* # bits / pixel		 */
	char          **annot,		/* -> annotation strings	 */
	int             fdo)		/* output file descriptor	 */
{
	BIH_T         **bihpp;		/* -> BI header pointer array	 */
	BIH_T          *bihp0;		/* -> band 0 BI header		 */
	STRVEC_T       *annotp;		/* -> annotation string vector	 */

 /*
  * make band 0 header
  */
	annotp = annot == NULL ? NULL : addsv((STRVEC_T *) NULL, annot[0]);

	bihp0 = bihmake(*nbytes, *nbits, (STRVEC_T *) NULL, annotp,
			(BIH_T *) NULL, nlines, nsamps, nbands);
	if (bihp0 == NULL) {
		error("can't make basic image header for band 0");
	}

	if (nbands < 2) {
		bihpp = &bihp0;
	}
 /*
  * if multi-band then make remaining headers
  */
	else {
		int             band;	/* current band #		 */

 /*
  * allocate header pointer array
  */
 /* NOSTRICT */
		bihpp = (BIH_T **) hdralloc(nbands, sizeof(BIH_T *),
					    ERROR, BIH_HNAME);
		if (bihpp == NULL) {
			error("can't allocate basic image header array");
		}
 /*
  * link headers into header pointer array
  */
		bihpp[0] = bihp0;

		for (band = 1; band < nbands; ++band) {
		   if (repeat) {
		      if (annotf) {
		         bihpp[band] = bihmake(*nbytes, *nbits,
		                      (STRVEC_T *) NULL, addsv( (STRVEC_T *)
							 NULL, annot[band]),
				       bihp0, nlines, nsamps, nbands);
		         if (bihpp[band] == NULL) {
		            error("can't make basic image header for band %d",
		                  band);
		         }
		      }
		      else {
				bihpp[band] = bihp0;
		      }
		   }
		   else {
		      bihpp[band] = bihmake(*++nbytes, *++nbits,
			            (STRVEC_T *) NULL, addsv((STRVEC_T *)
						       NULL, annot[band]),
			            bihp0, nlines, nsamps, nbands);
		      if (bihpp[band] == NULL) {
		         error("can't make basic image header for band %d",
		               band);
		      }
		   }
		}
	}
 /*
  * write the header
  */
	if (bihwrite(fdo, bihpp) != OK) {
		error("can't write basic image header");
	}

	if (boimage(fdo) != OK) {
		error("can't terminate header output");
	}
 /*
  * copy input to output
  */
	if (fdi != ERROR) {
		if (imgcopy(fdi, fdo) == ERROR) {
			warn("image copy failed");
		}
	}
}
