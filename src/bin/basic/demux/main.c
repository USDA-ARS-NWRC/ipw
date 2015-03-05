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
	static OPTION_T opt_b = {
		'b', "bands to extract",
		STR_OPTARGS, "band",
		REQUIRED,
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_b,
		&operands,
		0
	};

	RANGE_T       * ranges;		/* ranges of bands to demux	 */
	int		i;		/* loop index			 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * loop through arguments to -b option to get the list of bands selected
  */
	ranges = (RANGE_T *) ecalloc(n_args(opt_b), sizeof(RANGE_T));
	if (ranges == NULL) {
		error("Insufficient memory");
	}
	for (i = 0; i < n_args(opt_b); i++) {
	    char *p;

	    /*
	     *  Get integer for first band number
	     */
	    p = str_arg(opt_b, i);
	    if (! isdigit(*p)) {
		error("Missing band # in argument # %d for -b option", i+1);
	    }
	    while (isdigit(*p)) {
		p++;
	    }
	    if ((*p != EOS) && (*p != '-')) {
		error("Bad character '%c' in argument # %d for -b option", *p,
		      i+1);
	    }
	    ranges[i].start = (int) strtol(str_arg(opt_b, i), (char **) NULL,
					   10);

	    /*
	     *  If there was a following "-", check for optional
	     *  second band number.
	     */
	    if (*p == EOS) {
		ranges[i].end = ranges[i].start;
	    } else {
		char *s;

		p++;
		s = p;
		if (*p == EOS) {
		    ranges[i].end = -1;	/* denotes last band */
		} else {
		    while (isdigit(*p)) {
			p++;
		    }
		    if (*p != EOS) {
		      error("Bad character '%c' in argument # %d for -b option",
			     *p, i+1);
		    }
		    ranges[i].end = (int) strtol(s, (char **) NULL, 10);
		    if (ranges[i].end < ranges[i].start) {
			error("Start of band range (%d) after end (%d)",
			      ranges[i].start, ranges[i].end);
		    }
		}
	    }
	}  /* for */

 /*
  * access input file(s)
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
  * do it
  */
	demux(fdi, n_args(opt_b), ranges, fdo);

 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
