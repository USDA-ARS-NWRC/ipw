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
 * calculate or adjust coefficients, then re-map
 */

#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

void
coeffs(void)
{
	fpixel_t       *cfpval;		/* coefficients to be used	 */
	int             band;		/* band index			 */
	int             tbands;		/* i_bands * o_bands		 */
	int             i_bands;	/* # input bands		 */
	int             o_bands;	/* # output bands		 */

	i_bands = hnbands(parm.i_fd);
	o_bands = hnbands(parm.o_fd);
	tbands = i_bands * o_bands;
 /*
  * check coefficients & fill in defaults
  */
 /* NOSTRICT */
	cfpval = (fpixel_t *) ecalloc(tbands, sizeof(fpixel_t));
	assert(cfpval != NULL);
 /*
  * if all coefficients on command line, just copy
  */
	if (parm.ncoef != 0 && parm.ncoef % i_bands == 0) {
		for (band = tbands - 1; band >= 0; --band) {
			cfpval[band] = parm.coef[band];
		}
	}
 /*
  * else if no coefficients specified, use 1/nbands to compute mean
  */
	else if (parm.ncoef == 0) {
		fpixel_t        k = (double) 1 / (double) i_bands;

		for (band = tbands - 1; band >= 0; --band) {
			cfpval[band] = k;
		}
	}
 /*
  * else if one coefficient specified, use for all bands
  */
	else if (parm.ncoef == 1) {
		for (band = tbands - 1; band >= 0; --band) {
			cfpval[band] = parm.coef[0];
		}
	}
 /*
  * else error, # coeffs must be zero, 1, or modulo nbands
  */
	else {
		error("# coefficients specified = %d; %s",
		      parm.ncoef, "must be zero, 1, or modulo nbands");
	}
 /*
  * re-map input/output tables
  */
	parm.ftbl = re_map(cfpval);
	if (parm.ftbl == NULL) {
		error("can't re-map I/O lookup tables");
	}
	SAFE_FREE(cfpval);
}
