#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "input image file (default: stdin)",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "input kernel coefficient file (default: stdin)",
		STR_OPTARGS, "kernel",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_c,
		0
	};

	FILE *          kernelFile;	/* kernel file pointer		 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 2, 2, &opt_i, &opt_c);
 /*
  * access input file(s)
  */
	if (!got_opt(opt_i)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}

	no_tty(fdi);

	if (!got_opt(opt_c)) {
		kernelFile = stdin;
	}
	else {
		kernelFile = fopen(str_arg(opt_c, 0), "r");
		if (kernelFile == NULL) {
			error("can't open \"%s\"", str_arg(opt_c, 0));
		}
	}
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	convolve(fdi, kernelFile, fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
