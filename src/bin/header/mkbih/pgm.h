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

#ifndef	MKBIH_H
#define	MKBIH_H

#include "IPWmacros.h"

extern bool_t   getyi(int nbands, int n_i_nbytes, int *i_nbytes,
                      int n_i_nbits, int *i_nbits, int **o_nbytes,
		      int **o_nbits);
extern void     mkbih(int fdi, int nbands, bool_t repeat, bool_t annotf,
		      int nlines, int nsamps, int *nbytes, int *nbits,
		      char **annot, int fdo);

#define	DFLT_NBANDS	1
#define	DFLT_NBYTES	1
#define	DFLT_NBITS	8

#endif
