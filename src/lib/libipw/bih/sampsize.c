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
** NAME
**	sampsize -- compute total # bytes in image sample
**
** SYNOPSIS
**	#include "bih.h"
**
**	int
**	sampsize(
**		int		fd)	|* file descriptor		 *|
**
** DESCRIPTION
**	sampsize computes the total number of bytes per sample in the image
**	associated with the file descriptor fd
**
** RETURN VALUE
**	total # bytes per image sample
*/

#include "ipw.h"
#include "_bih.h"

int
sampsize(
	int		fd)		/* file descriptor		 */
{
	BIH_T         **bihpp;		/* -> BIH array			 */
	int             band;		/* loop counter			 */
	int             nbands;		/* # image bands		 */
	int             nbytes;		/* # bytes in image		 */

	bihpp = _bih[fd];
	assert(bihpp != NULL);

	nbands = bih_nbands(bihpp[0]);
	nbytes = 0;

	for (band = 0; band < nbands; ++band) {
		nbytes += bih_nbytes(bihpp[band]);
	}

	return (nbytes);
}

/* ------------------------------------------------------------------------ */

#ifdef TEST

main(argc, argv)
	int             argc;
	char          **argv;
{
	BIH_T         **bihpp;		/* -> BIH array			 */
	int             fdi;

	ipwenter(argc, argv, (OPTION_T **) NULL, "test sampsize() function");

	fdi = ustdin();
	no_tty(fdi);

	bihpp = bihread(fdi);
	if (bihpp == NULL) {
		error("can't read BIH");
	}

	(void) printf("%ld\n", sampsize(fdi));
}

#endif
