#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_s = {
		's', "negate bands",
		INT_OPTARGS, "bands",
		OPTIONAL, 1,
	};

	static OPTION_T opt_n = {
		'n', "bits per output pixel",
		INT_OPTARGS, "nbits",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&opt_s,
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
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operands, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * process arguments
  */
	parm.nbits = (got_opt(opt_n)) ? int_arg(opt_n, 0) : 0;
	parm.nsbands = n_args(opt_s);
	parm.sband = (parm.nsbands > 0) ? int_argp(opt_s) : NULL;
 /*
  * do it
  */
	headers(FALSE);
	iadd();
	headers(TRUE);
	output();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
