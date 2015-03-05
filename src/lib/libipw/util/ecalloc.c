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
**	ecalloc -- memory allocator (interface to UNIX calloc)
**
** SYNOPSIS
**	#include "ipw.h"
**
**	addr_t
**	ecalloc(
**		int    nelem,	|* # of elements in array	*|
**		int    elsize)	|* size of each element		*|
**
** DESCRIPTION
**	ecalloc() is the IPW interface to the UNIX function calloc().
**	It allocates an array of nelem elements, each of which is
**	elsize in size.
**	ecalloc checks that its arguments are positive nonzero values and
**	sets an IPW error condition if the allocation fails.
**
** RESTRICTIONS
**
** RETURN VALUE
**	pointer to allocated memory; else NULL if allocation failed
*/

#include "ipw.h"

addr_t
ecalloc(
	int		nelem,		/* # of elements in array	*/
	int		elsize)		/* size of each element		*/
{
	addr_t		rtn;

	assert(nelem > 0);
	assert(elsize > 0);

	rtn = calloc( (size_t) nelem, (size_t) elsize);
	if (rtn == NULL) {
		syserr();
	}

	return (rtn);
}
