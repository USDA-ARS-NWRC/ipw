#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_b = {
		'b', "begin line,sample",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_c = {
		'c', "center line,sample",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_e = {
		'e', "end line,sample",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_n = {
		'n', "output image size (#lines,#samples)",
		INT_OPTARGS, "size",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_w = {
		'w', "window specified re: this band's window header",
		INT_OPTARGS, "band",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_g = {
		'g', "window specified re: this band's geodetic header",
		INT_OPTARGS, "band",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_b,
		&opt_c,
		&opt_e,
		&opt_n,
		&opt_w,
		&opt_g,
		&operands,
		0
	};

	static XWSPEC_T  xw;		/* window specification (init 0) */

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 2, 4, &opt_b, &opt_c, &opt_e, &opt_n);
	opt_check(0, 1, 2, &opt_w, &opt_g);
 /*
  * collect options
  */
	if (got_opt(opt_b)) {
		xw.bline = real_arg(opt_b, 0);
		xw.bsamp = real_arg(opt_b, 1);

		xw.flags |= GOT_BEGIN;
	}

	if (got_opt(opt_c)) {
		xw.cline = real_arg(opt_c, 0);
		xw.csamp = real_arg(opt_c, 1);

		xw.flags |= GOT_CENTER;
	}

	if (got_opt(opt_e)) {
		xw.eline = real_arg(opt_e, 0);
		xw.esamp = real_arg(opt_e, 1);

		xw.flags |= GOT_END;
	}

	if (got_opt(opt_n)) {
		xw.nlines = int_arg(opt_n, 0);
		xw.nsamps = int_arg(opt_n, 1);

		xw.flags |= GOT_SIZE;
	}

	if (got_opt(opt_w)) {
		xw.band = int_arg(opt_w, 0);

		xw.flags |= GOT_XWIN;
	}
	else if (got_opt(opt_g)) {
		xw.band = int_arg(opt_g, 0);

		xw.flags |= GOT_XGEO;
	}
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
	window(fdi, &xw, fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
