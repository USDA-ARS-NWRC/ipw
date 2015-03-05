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

int
ltoi(
	long            i)	/* long to be converted to int	 */
{
	if (  (i < (long) INT_MIN)  ||  (i > (long) INT_MAX)  ) {
		error("%ld won't fit in an \"int\"", i);
	}

 /* NOSTRICT */
	return ((int) i);
}

#ifdef	TEST_MAIN
main()
{
	int             i;
	long            l;

	printf("(1) There should be no errors until (2):\n");

	l = INT_MIN;
	i = ltoi(l);
	assert((long) i == l);

	l = INT_MAX;
	i = ltoi(l);
	assert((long) i == l);

	if (sizeof(long) > sizeof(int)) {
		printf("(2) An error message should follow:\n");
		++l;
		i = ltoi(l);
	}
	else {
		printf("(2) done\n");
	}
}

#endif
