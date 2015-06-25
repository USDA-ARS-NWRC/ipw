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

	static OPTION_T opt_t = {
			't', "optical depth at z",
			REAL_OPTARGS, "tau",
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

	static OPTION_T opt_r = {
			'r', "mean surface albedo",
			REAL_OPTARGS, "R0",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
			's', "exoatmospheric solar irradiance",
			REAL_OPTARGS, "S0",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_u = {
			'u', "cosine solar zenith angle",
			REAL_OPTARGS, "mu0",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_n = {
			'n', "# output bits (default same as input)",
			INT_OPTARGS, "bits",
			OPTIONAL, 1, 2
	};

	static OPTION_T operands = {
			OPERAND, "input elevation image",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
			&opt_z,
			&opt_u,
			&opt_t,
			&opt_w,
			&opt_g,
			&opt_r,
			&opt_s,
			&opt_n,
			&operands,
			0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int             nbits[2];	/* # output bits		 */

	/*
	 * begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
	/*
	 * access input file(s)
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

	fdo = ustdout();
	no_tty(fdo);
	/*
	 * evaluate nbits
	 */
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
	/*
	 * read headers
	 */
	init_tau(fdi, fdo,
			real_arg(opt_z, 0), real_arg(opt_u, 0),
			real_arg(opt_t, 0), real_arg(opt_w, 0),
			real_arg(opt_g, 0), real_arg(opt_r, 0),
			real_arg(opt_s, 0), nbits);
	/*
	 * beam and diffuse radiation for elevation image
	 */
	elevrad(fdi, fdo);

	ipwexit(EXIT_SUCCESS);
}
