#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_v = {
		'v', "verbose output"
	};

	static OPTION_T opt_r = {
		'r', "raw values (don't convert to linear quantization)"
	};

	static OPTION_T opt_d = {
		'd', "input USGS DEM",
		STR_OPTARGS, "USGS_DEM",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_h = {
		'h', "input linear quantization header",
		STR_OPTARGS, "LQH",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_v,
		&opt_r,
		&opt_d,
		&opt_h,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * verbose option
  */
	parm.verbose = got_opt(opt_v);
 /*
  * raw values
  */
	parm.raw = got_opt(opt_r);
 /*
  * access input DEM file
  */
	if (!got_opt(opt_d)) {
		parm.i_fp = stdin;
	}
	else {
		parm.i_fp = fopen(str_arg(opt_d, 0), "r");
		if (parm.i_fp == NULL) {
			error("can't open \"%s\"", str_arg(opt_d, 0));
		}
	}

	no_tty(fileno(parm.i_fp));
 /*
  * access input LQH, if there is one, and set flag
  */
	if (!got_opt(opt_h)) {
		if (got_opt(opt_d)) {
			parm.h_fd = ustdin();
			parm.islqh = !isatty(parm.h_fd);
		}
		else {
			parm.islqh = FALSE;
		}
	}
	else {
		parm.h_fd = uropen(str_arg(opt_h, 0));
		if (parm.h_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_h, 0));
		}
		parm.islqh = TRUE;
	}
 /*
  * raw flags and specified LQH are not compatible
  */
	if (parm.raw && parm.islqh) {
		error("-r and -h (or LQH read from stdin) not compatible");
	}
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * do it
  */
	demhdr();
	profiles();
	headers();
	output();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
