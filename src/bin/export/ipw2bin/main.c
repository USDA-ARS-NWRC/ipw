#include "pgm.h"

#include "ipw.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_n = {
		'n', "output data value for masked points (default: 0.0)",
		REAL_OPTARGS, "nodata",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&opt_n,
		&operands,
		0
	};

	int		fdi;		/* input image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		fdo;		/* output file descriptor	 */
	float		nodata;		/* no data value		 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input image */

	if (!got_opt (operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty (fdi);

   /* access mask image */

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* get no data value */

	if (got_opt (opt_n)) {
		if (fdm == ERROR) {
			warn ("nodata value ignored without mask");
		}
		nodata = real_arg (opt_n, 0);
	} else {
		nodata = 0.0;
	}

   /* open output file - no ttys */

	fdo = ustdout();
	no_tty (fdo);

   /* read headers of input image */

	headers(fdi, fdm);

   /* read image/output binary data */

	ipw2bin(fdi, fdm, fdo, nodata);

   /* done */

	ipwexit (EXIT_SUCCESS);
}
