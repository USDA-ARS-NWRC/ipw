#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{

	static OPTION_T opt_d = {
		'd', "use inverse-distance weighting",
	};

	static OPTION_T opt_n = {
		'n', "# nearest neighbors for interpolation (default: 12)",
		INT_OPTARGS, "neighbors",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "header file (default: stdin)",
		STR_OPTARGS, "header",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
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
		&opt_h,
		&opt_m,
		&opt_p,
		0
	};

	FILE	       *ifp;		/* input file pointer		 */
	int		d1;		/* distance weighting flag	 */
	int		fdh;		/* header file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		nclosest;	/* # neighbors			 */
	int		npoints;	/* # points in input file	 */
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

	d1 = got_opt (opt_d);

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
	} else if (!got_opt(opt_h)) {
		error ("at least one of (-p,-h) must be specified");
	} else {
		ifp = stdin;
	}

   /* access header file */

	if (got_opt(opt_h)) {
		fdh = uropen (str_arg(opt_h, 0));
		if (fdh == ERROR) {
			error("can't open \"%s\"", str_arg(opt_h, 0));
		}
	} else {
		fdh = ustdin();
	}
	no_tty (fdh);

   /* access mask file */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* read input points */

	get_points (ifp, &points, &npoints, &min, &max);

	nclosest = MIN (npoints, nclosest);

   /* read input headers; write output headers */

	headers (fdh, fdm, fdo, min, max, &bline, &bsamp, &dline, &dsamp);

   /* do it */

	surface (fdm, fdo, d1, npoints, points, nclosest,
		 bline, bsamp, dline, dsamp);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
