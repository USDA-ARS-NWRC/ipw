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
**	ndig -- number of digits in an integer
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	ndig(
**		int i)
**
** DESCRIPTION
**	ndig calculates the number of decimal digits needed to represent the
**	value of i, including the minus sign for negative values.
**
** RESTRICTIONS
**
** RETURN VALUE
**	number of decimal digits in i
*/

#include "ipw.h"

int
ndig(
	REG_1 int       i)
{
	REG_2 int       n;

	if (i < 0) {
 /*
  * force i positive, just in case there's some wierdness with signed 0's
  */
		i = (-i);
		n = 2;
	}
	else {
		n = 1;
	}

 /*
  * I assume a few integer divisions are faster than (int) log10((double) i)
  */
	while ((i /= 10) != 0) {
		++n;
	}

	return (n);
}
