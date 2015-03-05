
#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "number of X,Y pairs in points file",
		INT_OPTARGS, "#points",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "clip input data to given X-values"
	};

	static OPTION_T opt_p = {
		'p', "points file (defaults to stdin)",
		STR_OPTARGS, "points_file",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "input image (defaults to stdin)",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "number of bits in output image",
		INT_OPTARGS, "#bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&opt_c,
		&opt_p,
		&opt_i,
		&opt_b,
		0
	};

	int		npoints;	/* # of X,Y point pairs in file	 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int		nbits;		/* #bits in output image	 */
	FILE	       *ifp;		/* file -> to points input file  */
	bool_t		clip;		/* flag to clip data to given X  */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input image */

	if (got_opt(opt_i)) {
		fdi = uropen (str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_i, 0));
		}
	} else {
		if (!got_opt(opt_p)) {
			fprintf (stderr,
				 "at least one of (-i,-p) must be specified");
			usage();
		}
		fdi = ustdin();
	}

	no_tty (fdi);

   /* access points input file */

	if (got_opt(opt_p)) {

		ifp = fopen (str_arg(opt_p, 0), "r");
		if (ifp == NULL) {
			error ("can't open \"%s\"", str_arg(opt_p, 0));
		}
	} else {
		ifp = stdin;
	}

   /* access output file */

	fdo = ustdout();
	no_tty (fdo);

   /* access arguments */

	npoints = int_arg (opt_n, 0);
	clip = got_opt (opt_c);

	if (got_opt (opt_b))
		nbits = int_arg (opt_b, 0);
	else
		nbits = 0;

   /* read/write headers */

	headers (fdi, fdo, nbits);

   /* do transform */

	iinterp (fdi, fdo, ifp, npoints, clip);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
