#include "ipw.h"
#include "pgm.h"

main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "# nearest neighbors for interpolation (default: 12)",
		INT_OPTARGS, "neibs",
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

        static OPTION_T opt_c = {
                'c', "do cross-validation; output to textfile",
		STR_OPTARGS, "textfile",
		OPTIONAL, 1, 1,
        };
 
	static OPTION_T opt_d = {
		'd', "nodata value (Default: -9999)",
		REAL_OPTARGS, "nodata value",
		OPTIONAL, 1, 1
	};


	static OPTION_T *optv[] = {
		&opt_p,
		&opt_c,
		&opt_n,
		&opt_d,
		&opt_h,
		&opt_m,
		0
	};

	FILE	       *ifp;		/* input file pointer		 */
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

	if (got_opt (opt_d)) {
		nodataval = real_arg (opt_d, 0);
	} else {
		nodataval = NODATA;
	}

	if (got_opt (opt_n)) {
		nclosest = int_arg (opt_n, 0);
	} else {
		nclosest = DEFAULT_NPTS;
	}

	if (got_opt (opt_c)) {
		jackflag = 1;
		jackfile = str_arg(opt_c, 0);
	} else {
		jackflag = 0;
	}

   /* access input file */

	if (got_opt(opt_p)) {
		ifp = fopen (str_arg(opt_p, 0), "r");
		if (ifp == NULL) {
			error("can't open \"%s\"", str_arg(opt_p, 0));
		}
	} else {
		if (jackflag == 1) 
		    error("can't use stdin for -p with the -j option");
		ifp = stdin;
	}

   /* access header file */

	if (jackflag == 0) {
	    if (got_opt(opt_h)) {
		fdh = uropen (str_arg(opt_h, 0));
		if (fdh == ERROR) {
			error("can't open \"%s\"", str_arg(opt_h, 0));
		}
	    } else {
		fdh = ustdin();
	    }
	    no_tty (fdh);
	} else {
	    if (got_opt(opt_h)) {
		error("can't use -h option with the -j option");
	    }
	}

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
	if (jackflag == 0) no_tty(fdo);

   /* read input points */

	get_points (ifp, &points, &npoints, &min, &max);

	if (nclosest > npoints)
	    error("neighbors cannot be larger than # of points in infile.");

   /* read input headers; write output headers */

	if (jackflag == 0) {
	    headers (fdh, fdm, fdo, min, max, &bline, &bsamp, &dline, &dsamp);
	}

   /* do it */

	crossval (fdm, fdo, npoints, points, nclosest,
		 bline, bsamp, dline, dsamp);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
