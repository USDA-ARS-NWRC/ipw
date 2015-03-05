
#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "class file",
		STR_OPTARGS, "class_file",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "input CRH",
		STR_OPTARGS, "crh",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
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

	static OPTION_T operands = {
		OPERAND, "input image file (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_h,
		&opt_m,
		&opt_f,
		&opt_c,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdh =ERROR;	/* input CRH file descriptor	 */
	int		fdm;		/* mask image file descriptor    */
	int             fdo;		/* output file descriptor	 */
	int		nclass;		/* # classes for output image	 */
	int		floor;		/* floor class value             */
	int		ceil;		/* ceiling class value           */
	int		gotcrh;		/* TRUE if input CRH given	 */
	CLASS	 	classes[MAXCLASS]; /*lo,hi,rep,class# for classes */
	FILE	       *ifp =NULL;	/* file -> to class input file   */


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

	if (got_opt(opt_f)) {
		if (!got_opt(opt_i))
			warn ("-f floor ignored if -i option not specified");
		floor = int_arg (opt_f, 0);
		if (floor < 0) {
			fprintf (stderr, "ERROR: floor must be positive\n");
			usage();
		}
	} else {
		floor = -1;
	}

	if (got_opt(opt_c)) {
		if (!got_opt(opt_i))
			warn ("-c ceil ignored if -r option not specified");
		ceil = int_arg (opt_c, 0);
		if (ceil < 0) {
			fprintf (stderr, "ERROR: ceil must be positive\n");
			usage();
		}
	} else {
		ceil = -1;
	}

   /* access input file or CRH */

	if (got_opt(opt_i)) {

		if (got_opt(opt_h))
			error ("options -h and -i are mutually exclusive");

		ifp = fopen (str_arg(opt_i, 0), "r");
		if (ifp == NULL) {
			error ("can't open \"%s\"", str_arg(opt_i, 0));
		}

		gotcrh = FALSE;

	} else if (got_opt(opt_h)) {

		fdh = uropen (str_arg(opt_h, 0));
		if (fdh == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_h, 0));
		}
		gotcrh = TRUE;

	} else {
		error ("at least one of -i,-h must be specified");
	}

   /* access mask file */

	if (got_opt(opt_m)) {

		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access output file */

	fdo = ustdout();
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdh, fdm, ifp, gotcrh, classes, &nclass,
		 &floor, &ceil, fdo);

   /* do classification */

	classify (fdi, fdm, nclass, classes, floor, ceil, fdo);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
