#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_z = {
		'z', "elevation of optical depth measurement",
		REAL_OPTARGS, "elev",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_w = {
		'w', "single-scattering albedo",
		REAL_OPTARGS, "omega",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_g = {
		'g', "scattering asymmetry parameter",
		REAL_OPTARGS, "gfact",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "exoatmospheric solar irradiance",
		REAL_OPTARGS, "S0",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "# output bits (default same as input)",
		INT_OPTARGS, "bits",
		OPTIONAL, 1, 2
	};

	static OPTION_T operands = {
		OPERAND, "input image",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_w,
		&opt_g,
		&opt_s,
		&opt_m,
		&opt_n,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	   */
	int             fdm;		/* mask image file descriptor	   */
	int             fdo;		/* output image file descriptor	   */
	int             nbits[2];	/* # output bits		   */
	double		z;		/* elevation of optical depth inst */
	double		omega;		/* single-scattering albedo	   */
	double		gfact;		/* scattering asymmetery parameter */
	double		S0;		/* exoatmospheric solar irradiance */


   /* begin */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (!got_opt(operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen(str_arg(operands, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(fdi);

   /* access mask file */

	if (got_opt(opt_m)) {
		fdm = uropen(str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* evaluate nbits */

	switch (n_args(opt_n)) {
	case 0:			/* neither, so both <- 0	 */
		nbits[0] = nbits[1] = 0;
		break;
	case 1:			/* band 0 set, so band[1] <- 0	 */
		nbits[0] = int_arg(opt_n, 0);
		nbits[1] = 0;
		break;
	case 2:			/* both set			 */
		nbits[0] = int_arg(opt_n, 0);
		nbits[1] = int_arg(opt_n, 1);
	}

	z = real_arg (opt_z, 0);
	omega = real_arg (opt_w, 0);
	gfact = real_arg (opt_g, 0);
	S0 = real_arg (opt_s, 0);

   /* read headers */

	headers (fdi, fdm, fdo, nbits);

   /* beam and diffuse radiation for elevation image */

	gelevrad (fdi, fdm, fdo, z, omega, gfact, S0);

	ipwexit (EXIT_SUCCESS);
}
