#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_m = {
		'm', "pixel,fpixel breakpoint pairs",
		REAL_OPTARGS, "val",
		REQUIRED, 2
	};

	static OPTION_T opt_u = {
		'u', "fpixel units of measure",
		STR_OPTARGS, "units",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "interpolation function (default: \"linear\")",
		STR_OPTARGS, "interp",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "band #s to receive map (default: all)",
		INT_OPTARGS, "band",
		OPTIONAL, 1
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
		&opt_m,
		&opt_u,
		&opt_i,
		&opt_b,
		&opt_f,
		&operands,
		0
	};

	char           *interp;		/* interpolation function name	 */
	char           *units;		/* fpixel units of measurement	 */
	fpixel_t       *fbkpt;		/* floating-point breakpoints	 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int             i;		/* loop counter			 */
	int             j;		/* loop counter			 */
	int             nbkpts;		/* # breakpoint pairs		 */
	pixel_t        *ibkpt;		/* integer breakpoints		 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * crack options
  */
	if (n_args(opt_m) & 1) {
		error("must specify pixel,fpixel pairs for -m");
	}

	nbkpts = n_args(opt_m) / 2;
 /* NOSTRICT */
	ibkpt = (pixel_t *) ecalloc(nbkpts, sizeof(pixel_t));
 /* NOSTRICT */
	fbkpt = (fpixel_t *) ecalloc(nbkpts, sizeof(fpixel_t));

	if (ibkpt == NULL || fbkpt == NULL) {
		error("can't allocate breakpoint arrays");
	}

	j = 0;
	for (i = 0; i < nbkpts; ++i) {
		ibkpt[i] = real_arg(opt_m, j);
		++j;
		fbkpt[i] = real_arg(opt_m, j);
		++j;
	}

	units = got_opt(opt_u) ? str_arg(opt_u, 0) : (char *) NULL;
	interp = got_opt(opt_i) ? str_arg(opt_i, 0) : (char *) NULL;
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
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	mklqh(fdi, nbkpts, ibkpt, fbkpt, units, interp, n_args(opt_b),
	      int_argp(opt_b), got_opt(opt_f), fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
