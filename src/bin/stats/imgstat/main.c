#include "ipw.h"
#include "crh.h"
#include "pgm.h"

int
main(
	int	argc,
	char  **argv)
{
	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_f = {
		'f', "obsolete option for compatibility with imgstat",
	};

	static OPTION_T opt_c = {
		'c', "output class statistics",
	};

	static OPTION_T opt_h = {
		'h', "class histogram output file",
		STR_OPTARGS, "hist_file",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_a = {
		'a', "abbreviated output"
	};

	static OPTION_T opt_u = {
		'u', "print units"
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&opt_c,
		&opt_f,
		&opt_h,
		&opt_a,
		&opt_u,
		&operands,
		0
	};

	int		fdi;		/* input image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	bool_t		hist;		/* flag to output class hist	 */
	int		nclass = 0;	/* # classes in classified band  */
	bool_t		class_stats;	/* flag for class stats		 */
	bool_t		abbrev;		/* flag for abbreviated output	 */
	bool_t		print_units;	/* flag to print units		 */
	FILE	       *hfp;		/* -> in class hist file	 */
	CLASS	       *classes;	/* class definitions		 */
	STAT            stat;		/* accum'ed stats for image	 */
	CLS_STAT        cls_stat;	/* accum'ed stats for classes	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file(s) */

	if (!got_opt (operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty (fdi);

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* check class statistics option */

	class_stats = got_opt (opt_c);

   /* check hist option */
   /* open hist file, if specified */

	hist = got_opt (opt_h);
	hfp = NULL;
	if (hist) {
		if (!class_stats) {
			warn ("-h option ignored; -c must also be specified");
			hist = FALSE;
		} else {
			hfp = fopen (str_arg(opt_h, 0), "w");
			if (hfp == NULL) {
				error ("can't open \"%s\"", str_arg(opt_h, 0));
			}
		}
	}

   /* check fast version option */

	if ( got_opt(opt_f) ) {
		warn ("-f option is obsolete");
	}

   /* check abbreviated output option */

	abbrev = got_opt (opt_a);

   /* check print-units option */

	print_units = got_opt (opt_u);

   /* read headers of input image(s) */

	stat.units   = NULL;
	headers (fdi, fdm, class_stats, &nclass, &classes, &(stat.units));
	if (stat.units == NULL) {
		stat.units = "";
	}

   /* compute/output statistics */

	stat.min     = 0.0;
	stat.max     = 0.0;
	stat.median  = 0.0;
	stat.mean    = 0.0;
	stat.sdev    = 0.0;
	stat.npixels = 0;

	cls_stat.min = cls_stat.max = NULL;
	cls_stat.sigma = cls_stat.sigma2 = NULL;
	cls_stat.median = NULL;
	cls_stat.mean = cls_stat.sdev = NULL;
	cls_stat.npixels = NULL;

	if (class_stats) {
	  class_statf (fdi, fdm, &stat, &cls_stat, nclass, hist, hfp);
	} else {
	  statf (fdi, fdm, &stat);
	}

	output (stat, abbrev, print_units, class_stats, cls_stat, nclass);

   /* done */

	ipwexit (EXIT_SUCCESS);
}
