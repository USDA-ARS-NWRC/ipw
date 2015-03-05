#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "# nearest neighbors for interpolation (default: 12)",
		INT_OPTARGS, "neighbors",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "output bits (default: 8)",
		INT_OPTARGS, "bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "dem image (optional mask as second band)",
		STR_OPTARGS, "dem",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_p = {
		'p', "input points file (default: stdin)",
		STR_OPTARGS, "infile",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_d,
		&opt_n,
		&opt_p,
		&opt_b,
		0
	};

	FILE	       *ifp;		/* input file pointer		 */
	int		fdi;		/* DEM image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int		nclosest;	/* # neighbors			 */
	int		npoints;	/* # points in input file	 */
	int		bits;		/* # bits in output file	 */
	double		bline;		/* line-coord of UL image corner */
	double		bsamp;		/* samp-coord of UL image corner */
	double		dline;		/* line spacing			 */
	double		dsamp;		/* sample spacing		 */
	float		min;		/* min data value in input	 */
	float		max;		/* max data value in input	 */
	struct point   *points;		/* input points			 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* process args */

	if (got_opt (opt_b)) {
		bits = int_arg (opt_b, 0);
	} else {
		bits = DEFAULT_BITS;
	}

	if (got_opt (opt_n)) {
		nclosest = int_arg (opt_n, 0);
	} else {
		nclosest = DEFAULT_NPTS;
	}

   /* access input file */

	if (got_opt(opt_p)) {
		ifp = fopen (str_arg(opt_p, 0), "r");
		if (ifp == NULL) {
			error("can't open \"%s\"", str_arg(opt_p, 0));
		}
	} else {
		ifp = stdin;
	}

   /* access DEM file */

	if (got_opt(opt_d)) {
		fdi = uropen (str_arg(opt_d, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_d, 0));
		}
	} else {
		fdi = ustdin();
	}

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* read input points */

	get_points (ifp, &points, &npoints, &min, &max);

	nclosest = MIN (npoints, nclosest);

   /* read input headers; write output headers */

	headers (fdi, fdo, min, max, &bline, &bsamp, &dline, &dsamp, bits);

   /* do it */

	deltaz (fdi, fdo, npoints, points, nclosest,
		 bline, bsamp, dline, dsamp);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
