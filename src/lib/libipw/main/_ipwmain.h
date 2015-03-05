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

#ifndef	_IPWMAIN_H
#define	_IPWMAIN_H

#define	DESC_DFLT	"?"
#define	PGM_DFLT	"(pgm)"

/*
 *  Global variables that are private to this module.
 */

extern char     **_argv;
extern char      *_descrip;
extern OPTION_T **_optv;

/*
 *  Functions that are private to this module.
 */

extern char	 *_cmdline(void);
extern bool_t     _cvt_opt_args(OPTION_T *opt, char *optargs);
extern char     **_getargs(int argc, char **argv, REG_2 OPTION_T **optv,
			   const char *descrip);

#endif  /* _IPWMAIN_H */
