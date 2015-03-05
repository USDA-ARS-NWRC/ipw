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

#include "ipw.h"
#include "_ipwmain.h"

/*
 *  These are the global variables internal to this directory.
 */


char **_argv = NULL;
/*
 *	_argv is a pointer to the program's vector of command-line argument
 *	strings.  _argv is initialized by getargs(), and is used to generate
 *	error and usage messages.
 */


char *_descrip = NULL;
/*
 *	_descrip is a pointer to string containing a brief (i.e. 1 line)
 *	description of the current program.  _descrip is initialized by
 *	ipwenter(), and is used by usage() to generate usage messages.
 */


OPTION_T **_optv = NULL;
/*
 *	_optv is a pointer to the program's vector of option descriptor
 *	structures.  _optv is initialized by getargs(), and is used to
 *	generate error and usage messages.
 */
