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
#include "bih.h"
#include "pgm.h"

int
main(
	int             argc,		/* # arguments			 */
	char          **argv)		/* ->'s to arguments		 */
{
	static OPTION_T opt_l = {
		'l', "# lines / image",
		INT_OPTARGS, "#lines",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "# samples / line",
		INT_OPTARGS, "#samps",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_a = {
		'a', "annotation string(s)",
		STR_OPTARGS, "string",
		OPTIONAL,
	};

	static OPTION_T opt_b = {
		'b', "# bands (pixels) / sample (default: 1)",
		INT_OPTARGS, "#bands",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_y = {
		'y', "# bytes / pixel (default: 1, or large enough for nbits)",
		INT_OPTARGS, "#bytes",
		OPTIONAL,
	};

	static OPTION_T opt_i = {
		'i', "# bits / pixel (default: nbytes * 8)",
		INT_OPTARGS, "#bits",
		OPTIONAL,
	};

	static OPTION_T opt_f = {
		'f', "force header output; ignore any input",
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_s,
		&opt_a,
		&opt_b,
		&opt_y,
		&opt_i,
		&opt_f,
		&operands,
		0
	};

	char          **annot;		/* -> annotation strings	 */
	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	int             nlines;		/* image # lines		 */
	int             nsamps;		/* # samples / line		 */
	int             nbands;		/* image # bands		 */
	int            *nbytes;		/* # bytes / pixel		 */
	int            *nbits;		/* # bits / pixel		 */
	bool_t          repeat;		/* ? repeat {nbytes,nbits}[0]	 */
	bool_t		annotf;		/* ? multiple band annotation    */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * process options
  */
	nlines = int_arg(opt_l, 0);
	nsamps = int_arg(opt_s, 0);
	nbands = got_opt(opt_b) ? int_arg(opt_b, 0) : DFLT_NBANDS;

	repeat = getyi(nbands, n_args(opt_y), int_argp(opt_y), n_args(opt_i),
		       int_argp(opt_i), &nbytes, &nbits);

	if (!got_opt(opt_a)) {
		annot = NULL;
	}
	else {
		if (n_args(opt_a) != nbands) {
			error("%d bands requires %d annotation strings",
			      nbands, nbands);
		}

		annot = str_argp(opt_a);
	}
  /* added by K. Longley 4/5/91 : */
  /* don't repeat band 0 header if multiple annotation strings given */
	if (n_args(opt_a) > 1)
		annotf = TRUE;
	else
		annotf = FALSE;


 /*
  * access input file
  */
	if (!got_opt(operands)) {
		fdi = got_opt(opt_f) ? ERROR : ustdin();
	}
	else {
		if (got_opt(opt_f)) {
			error("input file not allowed with \"-f\" option");
		}

		fdi = uropen(str_arg(operands, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(fdi);
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
	no_history(fdo);
 /*
  * do it
  */
	mkbih(fdi, nbands, repeat, annotf, nlines, nsamps, nbytes, nbits, 
	      annot, fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
