#include "ipw.h"
#include "pgm.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_n = {
			'n', "net radiation instead of incoming",
	};

	static OPTION_T opt_r = {
			'r', "Reflectance of the substrate",
			REAL_OPTARGS, "R0",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
			'd', "output diffuse as 2nd band",
	};

	static OPTION_T operands = {
			OPERAND, "beam & diffuse irrad, mu_s, Vf, Ct, albedo",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
			&opt_n,
			&opt_r,
			&opt_d,
			&operands,
			0
	};

	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	float		R0;		/* reflectance of the substrate	 */

	/*
	 * begin
	 */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 * access input files
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
	 * get options
	 */
	if (got_opt(opt_r))
		R0 = real_arg (opt_r, 0);
	else
		R0 = 0.0;
	/*
	 * read input data, calculate, and write
	 */

	toporad(fdi, fdo, got_opt(opt_n), got_opt(opt_r), got_opt(opt_d), R0);

	/*
	 * all done
	 */

	ipwexit(EXIT_SUCCESS);
}
