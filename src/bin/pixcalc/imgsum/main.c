#include "ipw.h"
#include "pgm.h"

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

	static OPTION_T operands = {
		OPERAND, "input image (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&operands,
		0
	};

	int		fdi;		/* input image file descriptor	*/
	int		fdm;		/* mask image file descriptor	*/


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file(s) */

	if (!got_opt (operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty (fdi);

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* read headers of input image(s) */

	headers (fdi, fdm);

   /* compute/output statistics */

	imgsum (fdi, fdm);

   /* done */

	ipwexit (EXIT_SUCCESS);
}
