#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_l = {
		'l', "minimum allowable value",
		REAL_OPTARGS, "lval",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_p = {
		'p', "times greater",
		REAL_OPTARGS, "mult",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_p,
		&opt_m,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		parm.im1 = ustdin();
	}
	else {
		parm.im1 = uropen(str_arg(operands, 0));
		if (parm.im1 == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.im1);
 /*
  * access mask image
  */
	if (got_opt(opt_m)) {
		parm.maskfile = uropen(str_arg(opt_m, 0));
		if (parm.maskfile == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		parm.maskfile = ERROR;
	}
 /*
  * set low data value
  */
	if (got_opt(opt_l)) {
		parm.lowpass = real_arg (opt_l, 0);
	} else {
		parm.lowpass = 0;
	}
 /*
  * set multiplier
  */
	if (got_opt(opt_p)) {
		parm.threshmult = real_arg (opt_p, 0);
	} else {
		parm.threshmult = 2;
	}
 /*
  * access output file
  */
	parm.outfile = ustdout();
	no_tty(parm.outfile);
 /*
  * process the headers
  */
	headers();
 /*
  * despike the image
  */
	despike();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
