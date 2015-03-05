#include "ipw.h"
#include "pgm.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_l = {
		'l', "line zoom factor",
		INT_OPTARGS, "factor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "sample zoom factor",
		INT_OPTARGS, "factor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_f = {
		'f', "zoom function type: mean, min or max",
		STR_OPTARGS, "type",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image (same dimensions as input image)",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_s,
		&opt_f,
		&opt_m,
		&operands,
		0
	};

	int		fdi;		/* input image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		fdo;		/* output image file descriptor	 */
	int		zline;		/* line zoom factor		 */
	int		zsamp;		/* sample zoom factor		 */
	int		ztype;		/* zoom type: MIN,MAX,or MEAN	 */


   /* Begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* Get and check options */

	zline = 1;
	if (got_opt(opt_l)) {
		zline = int_arg(opt_l, 0);
		if (zline <= 0) {
			error ("line zoom factor must be greater than zero");
		}
	} else if (!got_opt(opt_s)) {
		error ("at least one of (-l,-s) must be specified");
	}

	zsamp = 1;
	if (got_opt(opt_s)) {
		zsamp = int_arg(opt_s, 0);
		if (zsamp <= 0) {
			error ("sample zoom factor must be greater than zero");
		}
	}

	ztype = MEAN_VAL;
	if (got_opt(opt_f)) {
		if (!strcmp (str_arg(opt_f, 0), "mean")) {
			ztype = MEAN_VAL;
		} else if (!strcmp (str_arg(opt_f, 0), "min")) {
			ztype = MIN_VAL;
		} else if (!strcmp (str_arg(opt_f, 0), "max")) {
			ztype = MAX_VAL;
		} else {
			error ("Illegal resampling function: must be 'min', 'max', or 'mean'");
		}
	}

   /* Access input image */

	if (got_opt(operands)) {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	} else {
		fdi = ustdin();
	}

	no_tty (fdi);

   /* Access mask image */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* Access output image */

	fdo = ustdout();
	no_tty (fdo);

   /* Process headers */

	headers (fdi, fdm, fdo, zline, zsamp);

   /* Do all the work */

	resamp (fdi, fdm, fdo, zline, zsamp, ztype);

   /* All done */

	ipwexit (EXIT_SUCCESS);
}
