#include "ipw.h"

#include "pgm.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_o = {
			'o', "u,v coordinates of image origin",
			REAL_OPTARGS, "coord",
			REQUIRED, 2, 2
	};

	static OPTION_T opt_d = {
			'd', "u,v increment per line,sample",
			REAL_OPTARGS, "incr",
			REQUIRED, 2, 2
	};

	static OPTION_T opt_u = {
			'u', "u,v units of measure",
			STR_OPTARGS, "units",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_c = {
			'c', "u,v coordinate system identifier",
			STR_OPTARGS, "csys",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_b = {
			'b', "band #s to receive header (default: all)",
			INT_OPTARGS, "band",
			OPTIONAL, 1
	};

	static OPTION_T opt_f = {
			'f', "force header output; ignore any input image",
	};

	static OPTION_T operands = {
			OPERAND, "input image file",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
			&opt_o,
			&opt_d,
			&opt_u,
			&opt_c,
			&opt_b,
			&opt_f,
			&operands,
			0
	};

	char           *csys;		/* u,v coord sys identifier	 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	char           *units;		/* u,v units of measurement	 */

	/*
	 * begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
	/*
	 * crack options
	 */
	units = got_opt(opt_u) ? str_arg(opt_u, 0) : (char *) NULL;
	csys = got_opt(opt_c) ? str_arg(opt_c, 0) : (char *) NULL;
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
	mkgeoh(fdi, real_argp(opt_o), real_argp(opt_d), units, csys,
			n_args(opt_b), int_argp(opt_b), got_opt(opt_f), fdo);
	/*
	 * end
	 */
	ipwexit(EXIT_SUCCESS);
}
