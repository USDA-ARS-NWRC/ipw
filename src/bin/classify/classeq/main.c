
#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "# classes",
		INT_OPTARGS, "#classes",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image file",
		STR_OPERANDS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "lo,hi data values to use from input image",
		INT_OPTARGS, "val",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_f = {
		'f', "class value for data values less than lo",
		INT_OPTARGS, "floor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "class value for data values greater than hi",
		INT_OPTARGS, "ceil",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_r = {
		'r', "representative value: 0=median, 1=mean, 2=midpoint",
		INT_OPTARGS, "rep_code",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&opt_m,
		&opt_i,
		&opt_f,
		&opt_c,
		&opt_r,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdm;		/* mask image file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	int		nclass;		/* # classes for output image	 */
	fpixel_t	lo =0.0;	/* low value in input image      */
	fpixel_t	hi =0.0;	/* high value in input image     */
	int		floor;		/* floor class value             */
	int		ceil;		/* ceiling class value           */
	int		rep_method;	/* method for rep value		 */
	char	       *mask_file;	/* -> name of mask file		 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (!got_opt(operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty (fdi);

   /* process options */

	nclass = int_arg (opt_n, 0);
	if (nclass <= 0) {
		fprintf (stderr, "ERROR: nclass must be > 0\n");
		usage();
	}

	if (got_opt(opt_i)) {
		lo = int_arg (opt_i, 0);
		hi = int_arg (opt_i, 1);
	}

	if (got_opt(opt_f)) {
		if (!got_opt(opt_i))
			warn ("-f floor ignored if -i option not specified");
		floor = int_arg (opt_f, 0);
		if (floor < 0) {
			fprintf (stderr, "ERROR: floor must be positive\n");
			usage();
		} else if (floor > nclass) {
			fprintf (stderr, "ERROR: floor must be <= #classes");
			usage();
		}
	} else {
		floor = NO_DATA;
	}

	if (got_opt(opt_c)) {
		if (!got_opt(opt_i))
			warn ("-c ceil ignored if -r option not specified");
		ceil = int_arg (opt_c, 0);
		if (ceil < 0) {
			fprintf (stderr, "ERROR: ceil must be positive\n");
			usage();
		} else if (ceil > nclass) {
			fprintf (stderr, "ERROR: ceil must be <= #classes");
		}
	} else {
		ceil = NO_DATA;
	}

	if (got_opt(opt_r)) {
		rep_method = int_arg (opt_r, 0);
		if (rep_method < 0 || rep_method > 2) {
			fprintf (stderr, "ERROR: rep method must be 0, 1 or 2");
			usage();
		}
	} else {
		rep_method = MEDIAN;
	}

   /* access mask file */

	if (!got_opt(opt_m)) {
		fdm = ERROR;
		mask_file = NULL;
	} else {
		mask_file = str_arg (opt_m, 0);
		fdm = uropen (mask_file);
		if (fdm == ERROR) {
			error ("can't open \"%s\"", mask_file);
		}
	}

	no_tty(fdm);

   /* access output file */

	fdo = ustdout();
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdm, nclass, fdo);

   /* do it */

	classeq (fdi, fdm, fdo, nclass, got_opt(opt_i), lo, hi, floor, ceil,
		 str_arg(operands,0), mask_file, rep_method);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
