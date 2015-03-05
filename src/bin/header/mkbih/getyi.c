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
#include "pgm.h"

/*
 * getyi -- set up nbytes, nbits arrays
 */

bool_t
getyi(
	int             nbands,		/* # image bands		 */
	int             n_i_nbytes,	/* # values in i_nbytes		 */
	int            *i_nbytes,	/* nbytes from command line	 */
	int             n_i_nbits,	/* # values in i_nbits		 */
	int            *i_nbits,	/* nbits from command line	 */
	int           **o_nbytes,	/* -> output nbytes		 */
	int           **o_nbits)	/* -> output nbits		 */
{
	static int      dflt_nbits = DFLT_NBITS;
	static int      dflt_nbytes = DFLT_NBYTES;

	int             band;		/* loop counter			 */
	bool_t          repeat;		/* ? repeat band 0 initializers	 */

 /*
  * check for valid # nbytes,nbits initializers
  */
	if (n_i_nbytes > 1 && n_i_nbytes != nbands) {
		error("may specify either 1 or %d values for nbytes", nbands);
	}

	if (n_i_nbits > 1 && n_i_nbits != nbands) {
		error("may specify either 1 or %d values for nbits", nbands);
	}
 /*
  * combinations where 1st value may be repeated for all bands
  */
	if (n_i_nbytes == 0 && n_i_nbits == 0) {
		i_nbytes = &dflt_nbytes;
		i_nbits = &dflt_nbits;
		repeat = TRUE;
	}
	else if (n_i_nbytes == 0 && n_i_nbits == 1) {
		dflt_nbytes = (i_nbits[0] + CHAR_BIT - 1) / CHAR_BIT;
		i_nbytes = &dflt_nbytes;
		repeat = TRUE;
	}
	else if (n_i_nbytes == 1 && n_i_nbits == 0) {
		dflt_nbits = i_nbytes[0] * CHAR_BIT;
		i_nbits = &dflt_nbits;
		repeat = TRUE;
	}
	else if (n_i_nbytes == 1 && n_i_nbits == 1) {
		repeat = TRUE;
	}
 /*
  * all bands supplied on command line
  */
	else if (n_i_nbytes == nbands && n_i_nbits == nbands) {
		repeat = FALSE;
	}
 /*
  * build fake nbits array
  */
	else if (n_i_nbytes == nbands && n_i_nbits < 2) {
		dflt_nbits = n_i_nbits == 1 ? i_nbits[0] : 0;
 /* NOSTRICT */
		i_nbits = (int *) ecalloc(nbands, sizeof(int));
		if (i_nbits == NULL) {
			error("can't allocate nbits array");
		}

		for (band = 0; band < nbands; ++band) {
			i_nbits[band] = dflt_nbits != 0 ? dflt_nbits :
				i_nbytes[band] * CHAR_BIT;
		}

		repeat = FALSE;
	}
 /*
  * build fake nbytes array
  */
	else if (n_i_nbits == nbands && n_i_nbytes < 2) {
		dflt_nbytes = n_i_nbytes == 1 ? i_nbytes[0] : 0;
 /* NOSTRICT */
		i_nbytes = (int *) ecalloc(nbands, sizeof(int));
		if (i_nbytes == NULL) {
			error("can't allocate nbytes array");
		}

		for (band = 0; band < nbands; ++band) {
			i_nbytes[band] = dflt_nbytes != 0 ? dflt_nbytes :
				(i_nbits[band] + CHAR_BIT - 1) / CHAR_BIT;
		}

		repeat = FALSE;
	}
 /*
  * should have exhausted all possibilities ...
  */
	else {
		bug("programmer brain damage");
	}
 /*
  * check that nbits will fit in nbytes
  */
	if (repeat) {
		nbands = 1;
	}

	for (band = 0; band < nbands; ++band) {
		if (i_nbits[band] > i_nbytes[band] * CHAR_BIT) {
			error("%d bits won't fit in %d bytes",
			      i_nbits[band], i_nbytes[band]);
		}
	}

	*o_nbytes = i_nbytes;
	*o_nbits = i_nbits;

	return (repeat);
}
