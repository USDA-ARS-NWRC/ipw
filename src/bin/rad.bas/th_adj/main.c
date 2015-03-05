#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T operands = {
		OPERAND, "thermal irrad, air temperature, & cloud factor",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&operands,
		0
	};

	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
 /*
  * begin
  */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input files
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
  * read input data, calculate, and write
  */

	th_adj (fdi, fdo);

 /*
  * all done
  */

	ipwexit(EXIT_SUCCESS);
}
