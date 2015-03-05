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

#ifndef	_LQH_H
#define	_LQH_H

#include "lqh.h"

/* ------------------------------------------------------------------------- */

/*
 *  These are the global variables internal to this directory.
 */

extern LQH_T  ***_lqh;

/* ------------------------------------------------------------------------- */

/*
 *  These routines are internal to this directory.
 */

extern int      _lqharrays(LQH_T *lqhp);
extern bool_t   _lqhcheck(LQH_T **lqhpp, int nbands);
extern void     _lqhfree(int fd);
extern void     _lqhinit(void);
extern int      _lqhinterp(LQH_T **lqhpp, int nbands);

#endif
