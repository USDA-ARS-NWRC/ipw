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
  * Multiply each entry in the fpmap table by the coefficient for that
  * combination of input and output bands.  This allows indexing after
  * pvread().
  */

#include <string.h>

#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

fpixel_t ***
re_map(
	fpixel_t       *coefs)		/* -> mapping coefficients	 */
{
	FREG_1 fpixel_t rc;		/* coefficient			 */
	REG_1 fpixel_t *pm;		/* -> look-up table for band	 */
	REG_2 int       j;		/* index within look-up table	 */
	fpixel_t      **map;		/* input look-up tables		 */
	fpixel_t     ***ftbl;		/* diddled look-up tables	 */
	int             ib;		/* input band index		 */
	int             nbands;		/* # input bands		 */
	int             ob;		/* output band index		 */
	int             obands;		/* # output bands		 */
	int            *maplen;		/* lengths of look-up tables	 */

 /*
  * get mapping tables for input bands
  */
	map = fpmap(parm.i_fd);
	assert(map != NULL);
	maplen = fpmaplen(parm.i_fd);
	assert(maplen != NULL);
	assert(coefs != NULL);
 /*
  * make separate copy of mapping tables for each output band; we only
  * need these separate copies of bands 1 and beyond; for band 0 we
  * just use the one created by the fpio routines
  */
	obands = hnbands(parm.o_fd);
	nbands = hnbands(parm.i_fd);
 /* NOSTRICT */
	ftbl = (fpixel_t ***) ecalloc(obands, sizeof(fpixel_t **));
	assert(ftbl != NULL);
	ftbl[0] = map;

	for (ob = 1; ob < obands; ++ob) {
 /* NOSTRICT */
		ftbl[ob] = (fpixel_t **) ecalloc(nbands, sizeof(fpixel_t *));
		assert(ftbl[ob] != NULL);

		for (ib = 0; ib < nbands; ++ib) {
 /* NOSTRICT */
			ftbl[ob][ib] = (fpixel_t *) ecalloc(maplen[ib],
							    sizeof(fpixel_t));
			assert(ftbl[ob][ib] != NULL);
 /* NOSTRICT */
			(void) memcpy((char *) ftbl[ob][ib], (char *) map[ib],
				      (size_t) maplen[ib] * sizeof(fpixel_t));
		}
	}

 /*
  * multiply each map value by the coefficient for that combination of
  * input and output band
  */
	for (ob = 0; ob < obands; ++ob) {
		for (ib = 0; ib < nbands; ++ib) {
			rc = *coefs++;
			pm = ftbl[ob][ib];
			assert(pm != NULL);
			for (j = maplen[ib]; --j >= 0;) {
				*pm++ *= rc;
			}
		}
	}
 /*
  * return pointer to diddled look-up tables
  */
	return (ftbl);
}
