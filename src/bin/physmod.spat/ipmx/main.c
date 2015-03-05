#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input image",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&operand,
		0
	};

	int             fdi;		/* input image file descriptor	   */
	int             fdm;		/* mask image file descriptor	   */
	int             fdo;		/* output image file descriptor	   */


   /* begin */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

   /* open input file */

	if (!got_opt(operand)) {
		fdi = ustdin();
	} else {
		fdi = uropen(str_arg(operand, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operand, 0));
		}
	}

	no_tty(fdi);

   /* open mask file */

	if (got_opt(opt_m)) {
		fdm = uropen(str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* open output file */

	fdo = ustdout();
	no_tty(fdo);

   /* read headers */

	headers (fdi, fdm, fdo);

   /* calculate LE from Penman-Monteith combination method for image */

	ipmx (fdi, fdm, fdo);

	ipwexit (EXIT_SUCCESS);
}
