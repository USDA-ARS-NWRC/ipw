#include "ipw.h"
#include "bih.h"
#include "geoh.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_d = {
		'd', "date (year, month, day)",
		INT_OPTARGS, "i",
		REQUIRED, 3, 3
	};

	static OPTION_T opt_o = {
		'o', "prefix for output image names",
		STR_OPTARGS, "out_prefix",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_q = {
		'q', "# quadrature points (15 or 21; default: 15)",
		INT_OPTARGS, "npts",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "# bits in output image (default: CHAR_BIT)",
		INT_OPTARGS, "nbits",
		OPTIONAL, 1, 3
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "geodedic header (defaults to stdin)",
		STR_OPTARGS, "geoh",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_d,
		&opt_o,
		&opt_m,
		&opt_q,
		&opt_n,
		&operand,
		0
	};

	int		fdg;		/* geoh file descriptor		 */
	int		fdm;		/* mask file descriptor		 */
	int             kt;		/* # Kronrod pts		 */
	int             day;		/* day (1-31)		 	 */
	int             month;		/* month (1-12)		 	 */
	int             year;		/* year (1902-2037)	 	 */
	int		nlines;		/* # lines in output images	 */
	int		nsamps;		/* # samples in output images	 */
	int		nbits[OBANDS];	/* # bits in output image	 */
	int		i;		/* loop counter			 */
	double		blat;		/* start lat from geo header	 */
	double		blon;		/* start lon from geo header	 */
	double		dlat;		/* latitude pixel spacing	 */
	double		dlon;		/* longitude pixel spacing	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* arguments */

	year = int_arg (opt_d, 0);
	if (year < 1902 || year > 2037) {
		error ("Illegal year %d", year);
		usage();
	}
	month = int_arg (opt_d, 1);
	if (month < 1 || month > 12) {
		error ("Illegal month %d", month);
		usage();
	}
	day = int_arg (opt_d, 2);
	if (day < 1 || day > 31) {
		error ("Illegal day %d", day);
		usage();
	}

   /* only 15 or 21 Kronrod pts supported */

	if (got_opt(opt_q)) {
		kt = int_arg (opt_q, 0);
		switch (kt) {
			case 15:
			case 21:
				break;
			default:
				error("-q: only 15 or 21 pts supported");
		}
	} else {
		kt = DFLT_KT;
	}

   /* # bits in output image */

	switch (n_args(opt_n)) {
		case 0:
			for (i = 0; i < OBANDS; i++) {
				nbits[i] = CHAR_BIT;
			}
			break;
		case 1:
			for (i = 0; i < OBANDS; i++) {
				nbits[i] = int_arg (opt_n, 0);
			}
			break;
		case 3:
			for (i = 0; i < OBANDS; i++) {
				nbits[i] = int_arg (opt_n, i);
			}
			break;
		default:
			error ("must specify 1 or %d values for -n option",
				OBANDS);
	}

   /* access input geoh */

	if (got_opt (operand)) {
		fdg = uropen (str_arg(operand, 0));
		if (fdg == ERROR) {
			error ("can't open \"%s\"", str_arg(operand, 0));
		}
	} else {
		fdg = ustdin();
	}
	no_tty (fdg);

   /* access mask image */

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* read input headers */

	read_headers (fdg, fdm, &nlines, &nsamps, &blat, &blon, &dlat, &dlon);

   /* perform/output calculations */

	sunweights (fdm, str_arg(opt_o, 0), kt, year, month, day,
		    nlines, nsamps, nbits, blat, blon, dlat, dlon);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
