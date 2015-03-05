#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
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
  * do it
  */
	headers();
	hdspic();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
