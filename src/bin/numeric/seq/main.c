#include <string.h>
#include <math.h>

#include "pgm.h"
#include "ipw.h"

#define DEFAULT	100

int
main(
	int             argc,
	char           *argv[])
{
	static OPTION_T opt_r = {
		'r', "beginning and end of range of numbers",
		REAL_OPTARGS, "#",
		REQUIRED, 2, 2
	};

	static OPTION_T opt_n = {
		'n', "number of points (default 100)",
		INT_OPTARGS, "points",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "spacing between points",
		REAL_OPTARGS, "delta",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "spacing function: lin(default) log exp sqrt",
		STR_OPTARGS, "func",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_f = {
		'f', "C-type format (default is %g)",
		STR_OPTARGS, "fmt",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&opt_n,
		&opt_d,
		&opt_s,
		&opt_f,
		0
	};

	double          start;		/* start of range		 */
	double          fin;		/* end of range			 */
	double          inc =0.0;	/* increment			 */
	bool_t          okneg;		/* ? OK for # negative		 */
	bool_t          okzero;		/* ? OK for # zero		 */
	char           *fmt;		/* C-type format		 */
	char           *pct;		/* -> % in fmt			 */
	char           *sf =NULL;	/* spacing function		 */
	int             npts =0;	/* # points			 */

	double          (*inverse) ();	/* -> inverse function		 */
	double          (*xform) ();	/* -> transform function	 */

 /*
  * begin
  */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * how many points
  */

	if (got_opt(opt_n) && got_opt(opt_d)) {
		warn("-d specified, -n ignored");
	}
	else if (got_opt(opt_d)) {
		inc = real_arg(opt_d, 0);
		npts = 0;
	}
	else if (got_opt(opt_n)) {
		npts = ltoi(int_arg(opt_n, 0));
		if (npts <= 2) {
			error("-n too small, must be >= 3");
		}
	}
	else {
		npts = DEFAULT;
	}

 /*
  * output format
  */

	if (got_opt(opt_f)) {
		fmt = str_arg(opt_f, 0);

 /* check fmt to make sure valid, ending with e, f, or g */
		pct = strchr(fmt, '%');
		if (pct == NULL) {
			error("[ -f ..%..g.. or ..%..f.. or ..%..e.. ]");
		}
		if (strchr(pct, 'e') == NULL && strchr(pct, 'f') == NULL &&
		    strchr(pct, 'g') == NULL) {
			error("[ -f ..%..g.. or ..%..f.. or ..%..e.. ]");
		}
	}
	else {
		fmt = "%g";
	}

 /*
  * make sure range reasonable
  */

	start = real_arg(opt_r, 0);
	fin = real_arg(opt_r, 1);
	if (start == fin) {
		error("range error: lo equals hi");
	}

 /*
  * process spacing function and set transform and inverse funcs
  */

	if (got_opt(opt_s)) {
		sf = str_arg(opt_s, 0);
		if (streq(sf, "lin")) {
			xform = inverse = NULL;
			okneg = okzero = TRUE;
		}
		else if (streq(sf, "log")) {
			inverse = exp;
			xform = log;
			okneg = okzero = FALSE;
		}
		else if (streq(sf, "exp")) {
			inverse = log;
			xform = exp;
			okneg = okzero = TRUE;
		}
		else if (streq(sf, "sqrt")) {
			inverse = square;
			xform = sqrt;
			okneg = FALSE;
			okzero = TRUE;
		}
		else {
			error("%s: unrecognized spacing function", sf);
		}
	}
	else {
		okneg = okzero = TRUE;
		xform = inverse = NULL;
	}

 /*
  * check for bad negative or zero in range
  */

	if (!okneg && (start < (double) 0 || fin < (double) 0)) {
		error("no negative numbers with %s spacing", sf);
	}
	if (!okzero && (start == (double) 0 || fin == (double) 0)) {
		error("no non-positive numbers with %s spacing", sf);
	}

 /*
  * transform beginning and end and set increment
  */

	if (xform != NULL) {
		fin = (*xform) (fin);
		start = (*xform) (start);
	}
	if (npts != 0) {
		inc = (fin - start) / (npts - 1);
	}
	fin += inc / 2;

	if ( (inc >= DBL_MAX) || (inc <= DBL_MIN) ) {
		error("range error: increment out of range");
	}
	if ( (start >= DBL_MAX) || (start <= DBL_MIN) ) {
		error("range error: start out of range");
	}
	if ( (fin >= DBL_MAX) || (fin <= DBL_MIN) ) {
		error("range error: fin out of range");
	}
 /*
  * main loop, either forward or backward
  */

	seqloop(start, fin, inc, inverse, fmt);

 /*
  * all done
  */

	ipwexit(EXIT_SUCCESS);
}
