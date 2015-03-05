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
**	error -- print error message and exit
**
** SYNOPSIS
**	void error(format, arg, ...)
**	char *format;
**	{type} arg; ...
**
** DESCRIPTION
**	Error prints the error message described by the printf-style "format"
**	and "args" on the standard error output.  If usrerr() has been called,
**	its argument is also printed.  If syserr() has been called, a UNIX
**	system error message is also printed.
**
** WARNINGS
**	never returns
**
** APPLICATION USAGE
**	Call error() to report fatal errors (the program will exit after the
**	error messages are printed).
*/

/* LINTLIBRARY */

#include "ipw.h"
#include "_error.h"

#ifdef STANDARD_C
 
void error(const char *fmt, ...)
{
	va_list  ap;
 
	va_start(ap, fmt);
	_error("ERROR", fmt, ap);
	/* NOTREACHED */
	va_end(ap);
	ipwexit(EXIT_FAILURE);
}
 
#else  /* non standard C */
 
void error(va_alist)
va_dcl
{
	char     *fmt;
	va_list   ap;
 
	va_start(ap);
 
	fmt = va_arg(ap, char *);
	_error("ERROR", fmt, ap);
	/* NOTREACHED */
	va_end(ap);
	ipwexit(EXIT_FAILURE);
}
 
#endif  /* STANDARD_C */
