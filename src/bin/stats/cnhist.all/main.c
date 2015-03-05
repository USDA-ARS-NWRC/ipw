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

	static OPTION_T operands = {
		OPERAND, "input image(s)",
		STR_OPTARGS, "image",
		OPTIONAL, 1
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&operands,
		0
	};

	int		fdm;		/* mask image file descriptor	 */
	int		nfiles;		/* #input images		 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access mask image, if provided */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access arguments */

	nfiles = n_args (operands);

   /* do histogram */

	cnhist (nfiles, operands, fdm);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
