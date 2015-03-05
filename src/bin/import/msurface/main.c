#include "ipw.h"
#include "pgm.h"

PARM_T   parm;		/* global structure */

main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "# of interpolation points (default: 4)",
		INT_OPTARGS, "npoints",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_f = {
		'f', "nodata flag value (default: -9999)",
		REAL_OPTARGS, "nodata flag",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "output image bits (default: 8)",
		INT_OPTARGS, "bits",
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

	static OPTION_T opt_q = {
		'q', "min,max for lq header (default: min,max of point data)",
		REAL_OPTARGS, "val",
		OPTIONAL, 2, 2
	};

        static OPTION_T opt_g = {
                'g', "geo distance flag (default: Euclidean)",
        };

       static OPTION_T operand = {
                OPERAND, "input image made by msurfprep",
                STR_OPERANDS, "image",
                OPTIONAL, 1, 1,
        };

	static OPTION_T *optv[] = {
		&opt_p,
		&opt_n,
		&opt_b,
		&opt_f,
		&opt_m,
		&opt_q,
		&opt_g,
		&operand,
		0
	};



	FILE	       *ptfile;		/* file of point data            */
	int		fdi;		/* header file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		out_bits;       /* bits in output image          */
	int		nclosest;	/* # neighbors			 */
	int		inbands;        /* # bands in input file         */
	int		npoints;	/* # points in input file	 */
        int             Comp_pts();     /* compare function for qsort    */ 
	double		bline;		/* line-coord of UL image corner */
	double		bsamp;		/* samp-coord of UL image corner */
	double		dline;		/* line spacing			 */
	double		dsamp;		/* sample spacing		 */
	float		min;		/* min data value in input	 */
	float		max;		/* max data value in input	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* process args */

   /* access input image */

	if (got_opt(opt_p)) {
		ptfile = fopen (str_arg(opt_p, 0), "r");
		if (ptfile == NULL) {
			error("can't open \"%s\"", str_arg(opt_p, 0));
		}
	} 

   /* 'g', 'n', 'f' and 'b' options */

       if (got_opt (opt_g)) {
                parm.geoflag = 1;
        } else {
                parm.geoflag = 0;
        }

	if (got_opt(opt_n)) {
		nclosest = int_arg(opt_n, 0);
	} else {
	 	nclosest = 4;
	}

	if (got_opt(opt_b)) {
		out_bits = int_arg(opt_b, 0);
	} else {
	 	out_bits = 8;
	}

	if (got_opt(opt_f)) {
		naflag = real_arg(opt_f, 0);
	} else {
	 	naflag = -9999.0;
	}

   /* access input file */

	if (got_opt(operand)) {
		fdi = uropen (str_arg(operand, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operand, 0));
		}
	} else { error("input image must be specified and can not be stdin"); }

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

   /* get nlines/samps */

   /* read and sort input points */

	get_points (ptfile, &npoints, &min, &max);
  
   /* read input headers; write output headers */

	if (got_opt(opt_q)) {
		min = real_arg(opt_q, 0);
		max = real_arg(opt_q, 1);
		if (min >= max) error("min lq value must be < max");
	} 

	headers (fdi, fdm, fdo, min, max, &bline, &bsamp, &dline, &dsamp,
		 nclosest, out_bits, &inbands, ptfile, npoints);


   /* read nearest neighbor information from the look-up table */

	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);

   /* do it */

	surface (fdi, fdm, fdo, npoints, nclosest,
		 bline, bsamp, dline, dsamp, inbands);

   /* all done */

	ipwexit (0);
}

