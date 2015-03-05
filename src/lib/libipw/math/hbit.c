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
**	hbit -- find highest bit in integer
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	hbit(
**		unsigned        i)	|* unsigned integer	*|
**
** DESCRIPTION
**	hbit returns the highest bit set in the unsigned argument
**	passed it and returns that bit.  A return value of zero
**	indicates the value passed is zero.
**
**	hbit is normally called to find the number of bits needed to
**	hold an integer.
**
** RETURN VALUE
*/

#include "ipw.h"

int
hbit(
	unsigned        i)		/* unsigned integer	*/
{
	int             b;		/* bit counter	 */

	for (b = 0; i != 0; i >>= 1) {
		++b;
	}

	return (b);
}

#ifdef TEST_MAIN
#include <stdio.h>

main(argc, argv)
	int             argc;
	char          **argv;
{
	unsigned        x;

	for (;;) {
		printf("Enter an unsigned integer: ");
		if (scanf("%u", &x) == EOF)
			break;
		printf("%u, nbits = %d\n", x, hbit(x));
	}
	exit(0);
}

#endif
