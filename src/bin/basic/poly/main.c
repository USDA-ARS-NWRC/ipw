
#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_s = {
		's', "polygon fill seed line,sample",
		INT_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T operands = {
		OPERAND, "input vertices file",
		STR_OPERANDS, "file",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_s,
		&operands,
		0
	};

	FILE           *fpi =NULL;	/* input vertices file		 */
 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access files
  */
	if (!got_opt(operands)) {
		fpi = stdin;
	}
	else if (argc == 1) {
		fpi = fopen(str_arg(operands, 0), "r");
		if (fpi == NULL) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}
	else {
		usage();
	}
 /*
  * do it
  */
	if (got_opt(opt_s)) {
		poly(fpi, TRUE, int_arg(opt_s, 0), int_arg(opt_s, 1));
	}
	else {
		poly(fpi, FALSE, 0, 0);
	}
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
