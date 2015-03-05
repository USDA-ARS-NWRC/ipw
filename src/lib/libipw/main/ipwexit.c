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
**	ipwexit -- terminate an IPW program
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	ipwexit(
**		int  status)		|* exit status			 *|
**
** DESCRIPTION
**	ipwexit terminates the execution of an IPW program.  Open IPW files
**	are flushed and closed.  Status is passed to the operating system via
**	the system call "exit".
**
**	All IPW applications programs should call ipwexit as their last
**	executable statement.
**
** RETURN VALUE
**	This function does not return; it terminates the program by invoking
**	the system call "exit".
**
** GLOBAL VARIABLES READ
**
** GLOBAL VARIABLES MODIFIED
**
** BUGS
*/

#include "ipw.h"
#include "../uio/_uio.h"	/* for _uioexit() */

void
ipwexit(
	int             status)		/* exit status			 */
{
	static bool_t   called = FALSE;	/* ? called already		 */

	if (!called) {
 /*
  * prevent recursion (e.g. if error() is invoked in a library function;
  *                         error() happens to call ipwexit)
  */
		called = TRUE;

 /*
  * clean up any open IPW files
  */
		_uioexit();

	}

 /*
  * terminate execution
  */
	exit(status);
}
