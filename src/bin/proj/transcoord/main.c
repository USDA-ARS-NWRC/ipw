#include "ipw.h"
#include "mapproj.h"

#include "pgm.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "projection definition file for input coordinates",
		STR_OPTARGS, "inproj",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "projection definition file for output coordinates",
		STR_OPTARGS, "outproj",
		REQUIRED, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input file (defaults to stdin)",
		STR_OPTARGS, "infile",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_o,
		&operand,
		0
	};

	struct projdef *iproj;		/* input image proj parameters	 */
	struct projdef *oproj;		/* output image proj parameters	 */

	FILE	       *ifp;		/* file pointer to input file	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (got_opt(operand)) {
		ifp = fopen (str_arg(operand, 0), "r");
		if (ifp == NULL) {
			error ("error opening file %s", str_arg(operand, 0));
		}
	} else {
		ifp = stdin;
	}

   /* read projection definition files */

	iproj = read_proj (str_arg (opt_i, 0));
	if (iproj == NULL) {
		error ("error reading file %s", str_arg (opt_i, 0));
	}

	oproj = read_proj (str_arg (opt_o, 0));
	if (oproj == NULL) {
		error ("error reading file %s", str_arg (opt_o, 0));
	}

   /* translate coordinates */

	transcoord (ifp, iproj, oproj);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
