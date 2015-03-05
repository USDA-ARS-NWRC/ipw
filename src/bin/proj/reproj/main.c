#include "ipw.h"
#include "pgm.h"
#include "gctp.h"

char *rmethods[] = {
	"nn",
	"bi",
	"cc",
	"xx"
};

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "projection definition file for input image",
		STR_OPTARGS, "inproj",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "projection definition file for output image",
		STR_OPTARGS, "outproj",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_r = {
		'r', "rasampling method: bilinear (bi) or nearest neighbor (nn)",
		STR_OPTARGS, "resamp",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_g = {
		'g', "geodetic header for output image (default: stdin)",
		STR_OPTARGS, "geoh",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "upper-left line,sample coordinates for output image",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_d = {
		'd', "pixel size in line,sample directions",
		REAL_OPTARGS, "incr",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_l = {
		'l', "number of lines in output image",
		INT_OPTARGS, "lines",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "number of samples in output image",
		INT_OPTARGS, "lines",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image for the input image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input image (defaults to stdin)",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_o,
		&opt_r,
		&opt_g,
		&opt_b,
		&opt_d,
		&opt_l,
		&opt_s,
		&opt_m,
		&operand,
		0
	};

	int		fdi;		/* input image file descriptor	 */
	int		fdo;		/* output image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int		fdg;		/* geoh file descriptor		 */
	int		o_nlines = 0;	/* # lines in the output image	 */
	int		o_nsamps = 0;	/* # samples in the output image */
	int		resamp;		/* resampling method flag	 */
	double		o_bline;	/* UL output line coordinate	 */
	double		o_bsamp;	/* UL output sample coordinate	 */
	double		o_dline;	/* output pixel line increment	 */
	double		o_dsamp;	/* output pixel sample increment */
	double		i_bline;	/* UL input line coordinate	 */
	double		i_bsamp;	/* UL input sample coordinate	 */
	double		i_dline;	/* input pixel line increment	 */
	double		i_dsamp;	/* input pixel sample increment  */
	struct projdef *iproj;		/* input image proj parameters	 */
	struct projdef *oproj;		/* output image proj parameters	 */

   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input image */

	if (got_opt(operand)) {
		fdi = uropen (str_arg(operand, 0));
		if (fdi == ERROR) {
			error ("error opening file %s", str_arg(operand, 0));
		}
	} else {
		fdi = ustdin();
	}
	no_tty (fdi);

   /* get parameters */

	resamp = NN;
	if (got_opt(opt_r)) {
		if (!strcmp (str_arg(opt_r, 0), rmethods[NN])) {
			resamp = NN;
		} else if (!strcmp (str_arg(opt_r, 0), rmethods[BI])) {
			resamp = BI;
		} else if (!strcmp (str_arg(opt_r, 0), rmethods[CC])) {
			resamp = CC;
		} else if (!strcmp (str_arg(opt_r, 0), rmethods[XX])) {
			resamp = XX;
		} else {
			fprintf (stderr, "\nERROR: illegal resampling method: %s\n",
				 str_arg(opt_r,0));
			usage();
		}
	}

	if (got_opt(opt_g)) {
		fdg = uropen (str_arg(opt_g, 0));
		if (fdg == ERROR) {
			error ("error opening file %s", str_arg(opt_g, 0));
		}
	} else if (got_opt(operand)) {
		fdg = ustdin();
		if (isatty(fdg))
			fdg = ERROR;
	} else {
		fdg = ERROR;
	}

	if (fdg == ERROR) {
		if (!got_opt(opt_b) || !got_opt(opt_d) || !got_opt(opt_l) ||
		    !got_opt(opt_s)) {
			error ("must specify -g option or all of -b,-d,-l,-s options");
		}
		o_bline = real_arg(opt_b, 0);
		o_bsamp = real_arg(opt_b, 1);
		o_dline = real_arg(opt_d, 0);
		o_dsamp = real_arg(opt_d, 1);
		o_nlines = int_arg(opt_l, 0);
		o_nsamps = int_arg(opt_s, 0);

	} else if (got_opt(opt_b) || got_opt(opt_d) || got_opt(opt_l) ||
		    got_opt(opt_s)) {
		warn ("geoh (-g option) specified; -b,-d,-l,-s options ignored");
	}

   /* access mask image if provided */

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg (opt_m, 0));
		if (fdm == ERROR) {
			error ("error opening file %d", str_arg (opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access output image */

	fdo = ustdout();
	no_tty (fdo);

   /* read projection definition files */

	iproj = read_proj (str_arg (opt_i, 0));
	if (iproj == NULL) {
		error ("error reading file %s", str_arg (opt_i, 0));
	}

	oproj = read_proj (str_arg (opt_o, 0));
	if (oproj == NULL) {
		error ("error reading file %s", str_arg (opt_o, 0));
	}

   /* read input images headers; write output image headers */

	headers (fdi, fdg, fdm, fdo, o_nlines, o_nsamps, oproj->id, oproj->uid,
		 &o_bline, &o_bsamp, &o_dline, &o_dsamp,
		 &i_bline, &i_bsamp, &i_dline, &i_dsamp);

   /* reproject image */

	reproj (fdi, fdm, fdo, resamp, iproj, i_bline, i_bsamp, i_dline, i_dsamp,
			               oproj, o_bline, o_bsamp, o_dline, o_dsamp);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
