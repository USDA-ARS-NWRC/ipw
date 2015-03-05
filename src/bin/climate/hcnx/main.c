#include <string.h>
#include "ipw.h"
#include "pgm.h"

char *types[] = {
	"PRCP",
	"TMIN",
	"TMAX",
	"    ",
};
char *types_lc[] = {
	"prcp",
	"tmin",
	"tmax",
	"roff",
};

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "input file name",
		STR_OPTARGS, "infile",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "hydrological year order (default: calendar year)"
	};

	static OPTION_T opt_y = {
		'y', "year of data to extract (default: all)",
		INT_OPTARGS, "year",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "month of data to extract (default: all)",
		INT_OPTARGS, "month",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "day of data to extract (default: all)",
		INT_OPTARGS, "day",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_t = {
		't', "types(s) of data (prcp, tmin, tmax; default: all types in file)",
		STR_OPTARGS, "type",
		OPTIONAL, 1
	};

	static OPTION_T opt_a = {
		'a', "drainage area in square miles (required for runoff data)",
		REAL_OPTARGS, "drain_area",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "location (easting,northing) of station",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_h,
		&opt_y,
		&opt_m,
		&opt_d,
		&opt_t,
		&opt_a,
		&opt_l,
		0
	};

	char	       *filename;	/* input file path name		 */
	char	       *basename;	/* input file basename		 */
	int		i;		/* loop counter			 */
	int		t;		/* loop counter			 */
	int		year;		/* year of data to extract	 */
	int		month;		/* cal. month of data to extract */
	int		day;		/* day of data to extract	 */
	int		nxtract;	/* # data types to extract	 */
	bool_t		hydro_year;	/* flag for water years		 */
	bool_t		xtract[NTYPES];	/* flag to extract data type	 */
	bool_t		found;		/* flag for type matched	 */
	bool_t		got_loc;	/* flag if location specified	 */
	bool_t		runoff;		/* flag to extract runoff data	 */
	float		drain_area =0.0;/* drainage area in sq. miles	 */
	double		easting =0.0;	/* easting coord of station	 */
	double		northing =0.0;	/* northing coord of station	 */
	FILE	       *ifp;		/* input file pointer		 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	filename = str_arg (opt_i, 0);
	ifp = fopen (filename, "r");
	if (ifp == NULL) {
		error ("error opening file %s", filename);
	}

   /* check first character of filename to determine if temp/precip or runoff */
   /* first strip off directory name */

	basename = strrchr (filename, '/');
	if (basename == NULL)
		basename = filename;
	switch (basename[1]) {

		case 's':
			runoff = FALSE;
			break;
		case 'q':
		case 'x':
			runoff = TRUE;
			break;
		default:
			error ("unknown input file; s<ID>.dat or q<ID>.dat expected");
	}

	hydro_year = got_opt (opt_h);

   /* get year to extract */

	if (got_opt (opt_y)) {
		year = int_arg (opt_y, 0);
		if (year < MIN_YEAR || year > MAX_YEAR) {
			error ("Illegal year specified: %d", year);
		}
	} else {
		year = XTRACT_ALL;
	}

   /* get month to extract */

	if (got_opt (opt_m)) {
		month = int_arg (opt_m, 0);
		if (month < 1 || month > 12) {
			error ("Illegal month specified: %d", month);
		}

		/* convert water year date to calendar date */

		if (hydro_year) {
			month = month - 3;
			if (month < 0) {
				month += 12;
				if (year != XTRACT_ALL) {
					year--;
				}
			}
		}
	} else {
		month = XTRACT_ALL;
	}

   /* get day to extract */

	if (got_opt (opt_d)) {
		day = int_arg (opt_d, 0);
		if (day < 1 || day > 31) {
			error ("Illegal day specified: %d", day);
		}
	} else {
		day = XTRACT_ALL;
	}

   /* check for legal date */

	if (month != XTRACT_ALL && day != XTRACT_ALL) {
		if (day < 0 ||
		    day > numdays (MAX (1948, year), month)) {
			error ("Illegal date: %d/%d", month, day);
		}
	}

   /* if runoff to be extracted, make sure drainage area was specified */

	if (runoff) {
		if (got_opt (opt_a)) {
			drain_area = real_arg (opt_a, 0);
		} else {
			error ("-a drainage area required for runoff data");
		}
	}

   /* get types of data to extract from temp/precip file */

	nxtract = n_args (opt_t);
	if (!runoff) {
		if (nxtract <= 0) {
			for (i = 0; i < NTYPES; i++) {
				xtract[i] = TRUE;
			}
		} else {
			for (i = 0; i < NTYPES; i++) {
				xtract[i] = FALSE;
			}
			for (i = 0; i < nxtract; i++) {
				found = FALSE;
				for (t = 0; t < NTYPES; t++) {
					if (streq (types_lc[t], str_arg(opt_t, i))) {
						found = TRUE;
						xtract[t] = TRUE;
						break;
					}
				}
				if (!found) {
					error ("Illegal type: %s", str_arg(opt_t, i));
				}
			}
		}
	} else {
		if (nxtract > 0)
			warn ("-t type option ignored for runoff data");
	}

   /* get geographic location, if specified */

	if (got_opt (opt_l)) {
		easting = real_arg (opt_l, 0);
		northing = real_arg (opt_l, 1);
		got_loc = TRUE;
	} else {
		got_loc = FALSE;
	}

   /* do the work */

	hcnx (ifp, year, month, day, hydro_year, xtract, runoff, drain_area,
	      got_loc, easting, northing);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
