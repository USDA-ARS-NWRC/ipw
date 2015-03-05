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

#ifndef	DEMUX_H
#define	DEMUX_H

#include "IPWmacros.h"

/*
 *  A typedef representing a range of bands.
 */
typedef struct {
		int	start;	/* starting band		     */
		int	end;	/* ending band; -1 denotes last band */
} RANGE_T;


extern void     demux(int i_fd, int o_nbands, RANGE_T *ranges, int o_fd);
extern void     demuxhdrs(int i_fd, int i_nbands, int o_fd, int o_nbands,
			  int *inBands);
extern void     demuximg(int i_fd, int nlines, int nsamps, int i_nbands,
			 int o_fd, int o_nbands, int *inBands);

#endif
