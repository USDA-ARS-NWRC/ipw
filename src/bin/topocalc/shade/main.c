#include "ipw.h"
#include "pgm.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_a = {
			'a', "solar azimuth, degrees_ccw from south (+ E, - W)",
			REAL_OPTARGS, "azimuth",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_z = {
			'z', "solar zenith angle, degrees or",
			REAL_OPTARGS, "zenith",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
			'u', "cosine solar zenith angle",
			REAL_OPTARGS, "cos",
			OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
			OPERAND, "input slope/azimuth file",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
			&opt_z,
			&opt_u,
			&opt_a,
			&operand,
			0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

	/*
	 * begin
	 */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 * access input file
	 */
	if (!got_opt(operand)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_opnd(operand, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_opnd(operand, 0));
		}
	}

	no_tty(fdi);
	/*
	 * access output file
	 */
	fdo = ustdout();
	no_tty(fdo);

	/*
	 * initialize - read headers & allocate buffers
	 */

	init_shade(fdi, fdo, &opt_z, &opt_u, &opt_a);

	/*
	 * read input data, calculate sun angle, and write
	 */

	cosines(fdi, fdo);

	/*
	 * all done
	 */

	ipwexit(EXIT_SUCCESS);
}
