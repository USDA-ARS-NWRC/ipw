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
	static OPTION_T opt_a = {
		'a', "do bitwise AND of input bands"
	};

	static OPTION_T opt_o = {
		'o', "do bitwise OR of input bands"
	};

	static OPTION_T opt_x = {
		'x', "do bitwise XOR of input bands"
	};

	static OPTION_T opt_m = {
		'm', "last band is a mask"
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_a,
		&opt_o,
		&opt_x,
		&opt_m,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	void            (*op) () =NULL;	/* bitwise operation code	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 1, 3, &opt_a, &opt_o, &opt_x);

 /*
  * access input image file
  */
	if (!got_opt(operands)) {
		fdi = ustdin();
	}
	else {
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

 /*
  * set operator
  */
	if (got_opt(opt_a)) {
		op = bx_and;
	}
	else if (got_opt(opt_o)) {
		op = bx_or;
	}
	else if (got_opt(opt_x)) {
		op = bx_xor;
	}

 /*
  * do it
  */
	bitcom(fdi, got_opt(opt_m), op, fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
