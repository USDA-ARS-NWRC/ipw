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
 * BX_FUNC -- bitwise BX_OP of input bands
 */

void
BX_FUNC(
	pixel_t        *buf,		/* -> image buffer		 */
	REG_5 int       nsamps,		/* # samples in buffer		 */
	int             nbands)		/* # image bands		 */
{
	REG_1 pixel_t  *ip;		/* input pixel pointer		 */
	REG_4 int       nbands_1;	/* nbands - 1			 */
	REG_2 pixel_t  *op;		/* output pixel pointer		 */

	ip = buf;
	op = buf;

	nbands_1 = nbands - 1;

	do {
		REG_3 int       band;	/* band counter			 */

		*ip = *op++;

		band = nbands_1;
		do {
			*ip BX_OP *op++;
		} while (--band > 0);

		++ip;
	} while (--nsamps > 0);
}
