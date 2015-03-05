#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_c = {
		'c', "line,samp coefficients",
		REAL_OPTARGS, "coef",
		REQUIRED, 2
	};

	static OPTION_T opt_n = {
		'n',
		"# bits / output pixel (default based on #lines, #samples)",
		INT_OPTARGS, "#bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image header",
		STR_OPERANDS, "header",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_c,
		&opt_n,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input image file
  */
	if (!got_opt(operands)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_arg(operands, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(fdi);
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	wedge(fdi,
	       got_opt(opt_n) ? int_arg(opt_n, 0) : 0,
	       n_args(opt_c), real_argp(opt_c),
	       fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
