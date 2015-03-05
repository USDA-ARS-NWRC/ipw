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
**	_bug -- print bug message
**
** SYNOPSIS
**	void _bug(const char *msg, const char *file, int line)
**
** DESCRIPTION
**	_bug prints the bug message "msg", along with filename "file" and line
**	number "line", on the standard error output.  Program execution is
**	then terminated.
**
**	_bug is called by the "assert" and "bug" macros, which obtain the file
**	name and line number from the predefined macros __FILE__ and __LINE__,
**	respectively.
**
** WARNINGS
**	never returns
**
** APPLICATION USAGE
**	_bug is not meant to be called by applications programs.
*/

/* LINTLIBRARY */

#include "ipw.h"
#include "_error.h"

/*
 *  A local function which accepts a variable argument list, so it can
 *  pass the list onto the "_error" function.
 */

#ifdef STANDARD_C

static void
__bug(const char *fmt, ...)
{
	va_list  ap;

	va_start(ap, fmt);
	_error("BUG", fmt, ap);
	/* NOTREACHED */
	va_end(ap);
	ipwexit(EXIT_FAILURE);
}

#else  /* non standard C */

void
__bug(va_alist)
va_dcl
{
	char     *fmt;
	va_list   ap;

	va_start(ap);

	fmt = va_arg(ap, char *);
	_error("BUG", fmt, ap);
	/* NOTREACHED */
	va_end(ap);
	ipwexit(EXIT_FAILURE);
}

#endif  /* STANDARD_C */

/* ----------------------------------------------------------------------- */

void
_bug(
	const char     *msg,		/* message describing the bug	*/
	const char     *file,		/* file where bug is (__FILE__) */
	int             line)		/* line where bug is (__LINE__) */
{
	__bug("%s\n\t(file \"%s\", line %d)", msg, file, line);
}
