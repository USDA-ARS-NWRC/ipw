#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,		/* # arguments			 */
	char          **argv)		/* ->'s to arguments		 */
{
	static OPTION_T opt_b = {
		'b', "begin line,sample",
		REAL_OPTARGS, "coord",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_d = {
		'd', "line,sample increment",
		REAL_OPTARGS, "incr",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_f = {
		'f', "force header output; ignore any input image",
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_b,
		&opt_d,
		&opt_f,
		&operands,
		0
	};

	double          bline;		/* begin line #			 */
	double          bsamp;		/* begin sample #		 */
	double          dline;		/* line increment 		 */
	double          dsamp;		/* sample increment		 */
	int             fdi;		/* intput file descriptor	 */
	int             fdo;		/* output file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * process options
  */
	if (got_opt(opt_b)) {
		bline = real_arg(opt_b, 0);
		bsamp = real_arg(opt_b, 1);
	}
	else {
		bline = DFLT_BLINE;
		bsamp = DFLT_BSAMP;
	}

	if (got_opt(opt_d)) {
		dline = real_arg(opt_d, 0);
		dsamp = real_arg(opt_d, 1);
	}
	else {
		dline = DFLT_DLINE;
		dsamp = DFLT_DSAMP;
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
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	mkwinh(fdi, bline, bsamp, dline, dsamp, got_opt(opt_f), fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
