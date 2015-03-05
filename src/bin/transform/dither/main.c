#include "ipw.h"
#include "bih.h"

#include "pgm.h"

int
main(
	int             argc,
	char           **argv)
{
	static OPTION_T opt_r = {
		'r', "rank of dither matrix (default: 4)",
		INT_OPTARGS, "rank",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	int             rank;		/* dither matrix rank		 */
 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * collect options
  */
	if (got_opt(opt_r)) {
		rank = int_arg(opt_r, 0);
		switch (rank) {

		case 4:
		case 8:
		case 16:
			break;

		default:
			error("%d: illegal dither matrix rank", rank);
		}
	}
	else {
		rank = DFLT_RANK;
	}
 /*
  * access input file
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
	dither(fdi, rank, fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
