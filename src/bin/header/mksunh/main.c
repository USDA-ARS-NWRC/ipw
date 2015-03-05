#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,		/* # arguments			 */
	char          **argv)		/* ->'s to arguments		 */
{
	static OPTION_T opt_a = {
		'a', "solar azimuth from south",
		REAL_OPTARGS, "azimuth",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_z = {
		'z', "cosine of solar zenith angle",
		REAL_OPTARGS, "cos(zenith)",
		REQUIRED, 1, 1
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
		&opt_a,
		&opt_z,
		&opt_f,
		&operands,
		0
	};

	double          azimuth;	/* solar azimuth from south 	 */
	int             fdi;		/* intput file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	double          cos_zen;	/* cosine of solar zenith angle	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * process options
  */
	if (got_opt(opt_a)) {
		azimuth = real_arg(opt_a, 0);
	}
	else {
		error("must specify solar azimuth");
	}

	if (got_opt(opt_z)) {
		cos_zen = real_arg(opt_z, 0);
	}
	else {
		error("must specify cosine of solar zenith");
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
	mksunh(fdi, azimuth, cos_zen, got_opt(opt_f), fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
