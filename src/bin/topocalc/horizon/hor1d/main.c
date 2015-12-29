#include <math.h>
#include "ipw.h"

#include "bih.h"
#include <errno.h>
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_a = {
		'a', "direction of forward azimuth (degrees)",
		REAL_OPTARGS, "phi",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "grid spacing (normally obtained from GEOH header)",
		REAL_OPTARGS, "delta",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "backward direction (default: forward)"
	};

	static OPTION_T opt_z = {
		'z', "mask option, arg is solar zenith angle (degrees) or",
		REAL_OPTARGS, "zen",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
		'u', "mask option, arg is cosZ",
		REAL_OPTARGS, "cos",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "# bits / output pixel (default: 8)",
		INT_OPTARGS, "#bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T efile = {
		OPERAND, "input elevation image",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_a,
		&opt_b,
		&opt_z,
		&opt_u,
		&opt_d,
		&opt_n,
		&efile,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * azimuth
  */
	parm.azimuth = azmf(real_arg(opt_a, 0));
 /*
  * spacing
  */
	if (got_opt(opt_d)) {
		parm.spacing = real_arg(opt_d, 0);
		if (parm.spacing <= 0)
			error("-d %g: must be positive", parm.spacing);
	}
	else {
		parm.spacing = 0;
	}
 /*
  * mask option, solar zenith angle
  */
	if (got_opt(opt_u)) {
		errno = 0;
		parm.zenith = acos(real_arg(opt_u, 0));
		if (errno)
			error("bad return from acos()");
		if (got_opt(opt_z))
			warn("both -u and -z specified, -z over-ridden");
	}
	else if (got_opt(opt_z)) {
		parm.zenith = zenf(real_arg(opt_z, 0));
	}
	else {
		parm.zenith = 0;
	}
 /*
  * access input file
  */
	switch (n_args(efile)) {
	case 0:			/* no operand, open std input	 */

		parm.i_fd = ustdin();
		break;
	case 1:			/* open named file	 	 */
		parm.i_fd = uropen(str_opnd(efile, 0));
		if (parm.i_fd == ERROR)
			error("can't open \"%s\"", str_opnd(efile, 0));
		break;
	default:			/* too many operands	 	 */
		usage();
	}
 /*
  * can't read or write tty
  */
	no_tty(parm.i_fd);
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * # output bits, just 1 if mask
  */
	if (got_opt(opt_n)) {
		parm.nbits = int_arg(opt_n, 0);
	} else {
		parm.nbits = (parm.zenith != 0) ? 1 : CHAR_BIT;
	}

 /*
  * backward flag; default is forward
  */
	parm.backward = got_opt(opt_b);
 /*
  * process azimuth direction
  */
	if (parm.backward) {
		if (parm.azimuth >= 0)
			parm.azimuth -= M_PI;
		else
			parm.azimuth += M_PI;
	}
 /*
  * read/write headers
  */
	headers();
 /*
  * compute horizon
  */
	horizon();
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
