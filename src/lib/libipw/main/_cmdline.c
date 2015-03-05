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

/* LINTLIBRARY */

#include <string.h>

#include "ipw.h"
#include "_ipwmain.h"

/*
** NAME
**	_cmdline -- access command-line string
**
** SYNOPSIS
**	#include "_ipwmain.h"
**
**	char *_cmdline()
**
** DESCRIPTION
**	cmdline returns a pointer to a string containing the name and
**	arguments of the current IPW command, separated by spaces.
**
** RESTRICTIONS
**
** RETURN VALUE
**	pointer to the command-line string
**
** GLOBALS ACCESSED
**	_argv	pointer to command-line argument strings
**
** ERRORS
**	None.
**
** WARNINGS
**	Insufficient memory or inaccessible command-line arguments cause
**	assertion violations.
**
** APPLICATION USAGE
**	_cmdline is not meant to be called by IPW application programs.
**
**	bihwrite calls command line to generate a history record for the
**	current command.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

char *
_cmdline(void)
{
	static char    *rtn;		/* -> command line string	*/

	assert(_argv != NULL);

	if (rtn == NULL) {
		unsigned        len;	/* # bytes in command line str.	*/
		char          **p;	/* -> command line arguments	*/
 /*
  * compute # bytes needed to store command-line args, separated by spaces,
  * in an EOS-terminated string
  */
		len = 0;
		for (p = _argv; *p != NULL; ++p) {
			len += strlen(*p) + 1;
		}
 /*
  * allocate command-line string
  */
		rtn = ecalloc((int) len, sizeof(char));
		assert(rtn != NULL);
 /*
  * concatenate command-line args into string
  */
		rtn[0] = EOS;
		p = _argv;

		for (;;) {
			(void) strcat(rtn, *p);

			if (*++p == NULL) {
				break;
			}

			(void) strcat(rtn, " ");
		}
	}

	return (rtn);
}
