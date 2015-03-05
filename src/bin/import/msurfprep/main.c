#include "ipw.h"
#include "pgm.h"
PARM_T   parm;  /* global structure */

main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "# nearest neighbors to be stored (default: 12)",
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
		'p', "file of interpolation points",
		STR_OPTARGS, "points file",
		REQUIRED, 1, 1
	};

        static OPTION_T opt_g = {
                'g', "geo distance flag (default: Euclidean)",
        };


	static OPTION_T *optv[] = {
		&opt_n,
		&opt_h,
		&opt_p,
		&opt_g,
		0
	};

	FILE	       *ptfile;		/* file of point data            */
	int		fdh;		/* header file descriptor	 */
	int		fdo;		/* output file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		nneibs;	        /* # neighbors			 */
	int		nlines; 	/* # lines in hdr image  	 */
	int		nsamps; 	/* # samps in hdr image  	 */
	int		npoints;	/* # points in ptfile    	 */
	double		bline;		/* line-coord of UL image corner */
	double		bsamp;		/* samp-coord of UL image corner */
	double		dline;		/* line spacing			 */
	double		dsamp;		/* sample spacing		 */

   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* process args */

	if (got_opt (opt_n)) {
		nneibs = int_arg (opt_n, 0);
	} else {
		nneibs = DEFAULT_NPTS;
	}

        if (got_opt (opt_g)) {
                parm.geoflag = 1;
        } else {
                parm.geoflag = 0;
        }

   /* access input/output files */

	if (got_opt(opt_p)) {
		ptfile = fopen (str_arg(opt_p, 0), "r");
		if (ptfile == NULL) {
			error("can't open \"%s\"", str_arg(opt_p, 0));
		}
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
 


   /* read input headers; write output headers */

    headers (fdh, fdm, fdo, &bline, &bsamp, &dline, &dsamp, nneibs, 
	ptfile, &npoints);

	nlines = hnlines(fdh);
	nsamps = hnsamps(fdh);

   /* do it */

   	surfaceprep(nlines, nsamps, nneibs, bline, bsamp, dline, dsamp,
		npoints, fdo);

   /* write the output file */

   free(points); free(closest);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
