#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_t = {
		't', "date/time: year, month, day, hour, [min, sec]",
		INT_OPTARGS, "i",
		REQUIRED, 4, 6
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "geodetic header (defaults to stdin)",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_t,
		&opt_m,
		&operand,
		0
	};

	int		fdg;		/* geo header file descriptor	 */
	int		fdm;		/* mask file descriptor		 */
	int		fdo;		/* output image file descriptor	 */
	int             day;		/* day (1-31)		 	 */
	int             month;		/* month (1-12)		 	 */
	int             year;		/* year 	 	 	 */
	int		hour;		/* hour (0-23)			 */
	int		min;		/* min (0-59)			 */
	int		sec;		/* second (0-59)		 */
	double		blat;		/* lat at UL of image (deg)	 */
	double		blon;		/* lon at UL of image (deg)	 */
	double		dlat;		/* latitude spacing (deg)	 */
	double		dlon;		/* longitude spacing (deg)	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* arguments */

	year = int_arg (opt_t, 0);
	if (year < 0) {
		error ("Illegal year %d", year);
		usage();
	}
	month = int_arg (opt_t, 1);
	if (month < 1 || month > 12) {
		error ("Illegal month %d", month);
		usage();
	}
	day = int_arg (opt_t, 2);
	if (day < 1 || day > 31) {
		error ("Illegal day %d", day);
		usage();
	}
	hour = int_arg (opt_t, 3);
	if (hour < 0 || hour > 23) {
		error ("Illegal hour %d", hour);
		usage();
	}
	if (n_args(opt_t) > 4) {
		min = int_arg (opt_t, 4);
		if (min < 0 || min > 59) {
			error ("Illegal minute %d", min);
			usage();
		}
	} else {
		min = 0;
	}
	if (n_args(opt_t) > 5) {
		sec = int_arg (opt_t, 5);
		if (sec < 0 || sec > 59) {
			error ("Illegal second %d", sec);
			usage();
		}
	} else {
		sec = 0;
	}

   /* access input geodetic header */

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

   /* access output image */

	fdo = ustdout();
	no_tty (fdo);

   /* read/write headers */

	headers (fdg, fdm, fdo, &blat, &blon, &dlat, &dlon);

   /* perform/output calculations */

	gsunlight (fdm, fdo, year, month, day, hour, min, sec,
		   blat, blon, dlat, dlon);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
