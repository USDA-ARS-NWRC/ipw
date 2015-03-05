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
	int             argc,
	char          **argv)
{
	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		REQUIRED, 1,
	};

	static OPTION_T *optv[] = {
		&operands,
		0
	};

	int             i;		/* current input file #		 */
	int            *i_fdp;		/* -> input file descriptor
					 * array */
	int             i_n;		/* # input files		 */
	int             o_fd;		/* output file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input files
  */
	i_n = n_args(operands);

 /* NOSTRICT */
	i_fdp = (int *) ecalloc(i_n, sizeof(int));
	if (i_fdp == NULL) {
		error("can't allocate input file descriptor array");
	}

	for (i = 0; i < i_n; ++i) {
		i_fdp[i] = uropen(str_arg(operands, i));
		if (i_fdp[i] == ERROR) {
			error("can't open \"%s\"", str_arg(operands, i));
		}

		no_tty(i_fdp[i]);
	}

 /*
  * access output file
  */
	o_fd = ustdout();
	no_tty(o_fd);

 /*
  * turn off history mechanism
  */
	no_history(o_fd);

 /*
  * do it
  */
	mux(i_n, i_fdp, o_fd);

 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
