#include <string.h>
#include <unistd.h>

#include "ipw.h"
#include "pgm.h"

char *day_units = "days";
char *hr_units  = "hours";
char *sec_units = "seconds";

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "input format (predefined name or format string)",
		STR_OPTARGS, "infmt",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "output format (predefined name or format string)",
		STR_OPTARGS, "outfmt",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_y = {
		'y', "year",
		INT_OPTARGS, "year",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "from GMT to local time zone (minutes west of Greenwich)",
		INT_OPTARGS, "zone",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_g = {
		'g', "from local time zone to GMT (minutes west of Greenwich)",
		INT_OPTARGS, "zone",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "time in hours (for formats: jd, jd1, wy, wy1)"
	};

	static OPTION_T opt_s = {
		's', "time in seconds (for formats: jd, jd1, wy, wy1)",
	};

	static OPTION_T operand = {
		OPERAND, "date-time to convert",
		STR_OPERANDS, "date-time",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_o,
		&opt_y,
		&opt_l,
		&opt_g,
		&opt_h,
		&opt_s,
		&operand,
		0
	};

	char	       *in_fmt;		/* input format: name or string	 */
	char	       *out_fmt;	/* output format: name or string */
	int		in_fmt_type;	/* type of input format		 */
	int		out_fmt_type;	/* type of output format	 */
	int		year =0;	/* year (2 digits)		 */
	int		zone;		/* minutes from Greenwich	 */
	bool_t		local_in;	/* flag for local time to GMT	 */
	bool_t		local_out;	/* flag for GMT to local time	 */
	bool_t		hrs;		/* flag for time in hours	 */
	bool_t		secs;		/* flag for time in seconds	 */
	bool_t		crt;		/* flag for stdin from tty	 */
	bool_t		eof_read;	/* flag for EOF read in stdin	 */
	char	       *units;		/* time units string		 */
	char		line[MAX_INPUT];/* buffer for input line	 */



   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* get/check runstring args */

	hrs = got_opt(opt_h);
	secs = got_opt(opt_s);

	if (hrs && secs) {
	    error("-h and -s options are mutually exclusive");
	}

	zone = 0.0;
	if (got_opt (opt_l)) {
	    local_out = TRUE;
	    zone = int_arg (opt_l, 0);
	} else {
	    local_out = FALSE;
	}

	if (got_opt (opt_g)) {
	    if (local_out) {
		error("-g and -l options are mutually exclusive");
	    }
	    local_in = TRUE;
	    zone = int_arg (opt_g, 0);
	} else {
	    local_in = FALSE;
	}

   /* get input format */

	if (got_opt(opt_i)) {
	    in_fmt = str_arg(opt_i, 0);
	    if (STREQ(in_fmt, "jd"))
		in_fmt_type = JD;
	    else if (STREQ(in_fmt, "jd1"))
		in_fmt_type = JD1;
	    else if (STREQ(in_fmt, "wy"))
		in_fmt_type = WY;
	    else if (STREQ(in_fmt, "wy1"))
		in_fmt_type = WY1;
	    else if (STREQ(in_fmt, "date"))
		in_fmt_type = DATE;
	    else {
		in_fmt_type = FMTSTR;
	    }
	} else {
	    in_fmt_type = FMTSTR;
	    in_fmt = DEFAULT_FMTSTR;
	}

	switch (in_fmt_type) {
	    case JD :
	    case JD1 :
	    case WY :
	    case WY1 :
		if (got_opt (opt_y))
		    year = int_arg (opt_y, 0);
		else
		    error("Must specify -y year option");
		break;
	}

   /* get output format */

	if (got_opt(opt_o)) {
	    out_fmt = str_arg(opt_o, 0);
	    if (STREQ(out_fmt, "jd"))
		out_fmt_type = JD;
	    else if (STREQ (out_fmt, "jd1"))
		out_fmt_type = JD1;
	    else if (STREQ (out_fmt, "wy"))
		out_fmt_type = WY;
	    else if (STREQ (out_fmt, "wy1"))
		out_fmt_type = WY1;
	    else if (STREQ (out_fmt, "date"))
		out_fmt_type = DATE;
	    else {
		out_fmt_type = FMTSTR;
	    }
	} else {
	    out_fmt_type = FMTSTR;
	    out_fmt = DEFAULT_FMTSTR;
	}

	if (STREQ(in_fmt, out_fmt))
	    error("Input and output formats are the same");


   /* check for one operand */

	if (got_opnds(operand)) {
	     cvtime(in_fmt_type, in_fmt, out_fmt_type, out_fmt, year, local_in,
		     local_out, zone, hrs, secs, str_opnd(operand, 0));
	} else {

   /* check stdin for re-direct */

	    crt = isatty(STDIN_FILENO);

   /* get units string for crt prompt */

    	    if (hrs)
		units = hr_units;
	    else if (secs)
		units = sec_units;
	    else
		units = day_units;

   /* loop reading input file */

	    eof_read = FALSE;
	    while (!eof_read) {
		if (crt) {
		    switch (in_fmt_type) {
			case JD:
			case JD1:
			    fprintf(stderr, "input julian time (%s): ", units);
			    break;

			case WY:
			case WY1:
			    fprintf(stderr, "input water year time (%s): ",
				    units);
			    break;

			default:
			    fprintf(stderr, "input date and time: ");
			    break;
		    } /* switch */
		}

		if (fgets(line, MAX_INPUT, stdin) != NULL) {
		    /*
		     *  Trim terminating newline.  If none, the input line
		     *	was truncated, so warn user.
		     */
		    int i = strlen(line);
		    if (i > 0) {
			i--;
			if (line[i] == '\n') {
			    line[i] = EOS;
			} else {
			    warn("Input line with more than %d characters truncated", MAX_INPUT-1);
			}
		    }

	            eof_read = cvtime(in_fmt_type, in_fmt, out_fmt_type,
				      out_fmt, year, local_in, local_out,
				      zone, hrs, secs, line);
		} else {
		    eof_read = TRUE;
		}
	    } /* while reading */
	} /* else */

   /* all done */

	ipwexit(EXIT_SUCCESS);
}
