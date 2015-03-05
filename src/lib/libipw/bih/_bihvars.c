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
#include "_bih.h"

/* ------------------------------------------------------------------------ */

/*
 *  These are the public global-variables in this directory.
 */

char *o_byteorder = NULL;
/*
 *	o_byteorder is the byte order for output images.  Initialized 
 *	with the hostorder() function; may be changed after the call
 *	ipwenter() if a IPW command plans to bypass the pixio byte swapping
 *	layer (e.g., transpose, flip, and window).
 */

/* ------------------------------------------------------------------------ */

/*
 *  These are the global variables internal to this directory.
 */

BIH_T ***_bih = NULL;
/*
 *	_bih is an array of double-indirect pointers to basic image
 *      headers, indexed by the corresponding UNIX file descriptor.
 */

bool_t * _no_hist = NULL;
/*
 *	_no_hist is an array of flags to suppress printing of the
 *	history records
 */
