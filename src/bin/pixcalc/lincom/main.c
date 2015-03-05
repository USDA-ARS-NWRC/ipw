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
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_c = {
		'c', "band coefficients (default 1/nbands)",
		REAL_OPTARGS, "coef",
		OPTIONAL, 1
	};

	static OPTION_T opt_n = {
		'n', "# bits / output pixel (default from input image)",
		INT_OPTARGS, "#bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_c,
		&opt_n,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operands, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * process arguments
  */
	parm.nbits = got_opt(opt_n) ? int_arg(opt_n, 0) : 0;
	parm.ncoef = n_args(opt_c);
	parm.coef = (parm.ncoef > 0) ? real_argp(opt_c) : NULL;
 /*
  * do it
  */
	headers(FALSE);
	coeffs();
	lincom();
	headers(TRUE);
	output();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
