#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_l = {
		'l', "flip order of lines in file"
	};

	static OPTION_T opt_s = {
		's', "flip order of samples within lines"
	};

	static OPTION_T operand = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_s,
		&operand,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access input file
  */
	if (!got_opt(operand)) {
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operand, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operand, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * collect options
  */
	parm.lines = got_opt(opt_l);
	parm.samps = got_opt(opt_s);

	if (!parm.lines && !parm.samps) {
		error("must specify -l or -s (or both)");
	}
 /*
  * do it
  */
	headers();
	flip();
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
