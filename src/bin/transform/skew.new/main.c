#include <omp.h>

#include "../skew.new/pgm.h"
#include "ipw.h"
#include "skewh.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_a = {
			'a', "skew angle",
			REAL_OPTARGS, "angle",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
			'h', "ignore input skew header",
	};

	static OPTION_T opt_P = {
			'P', "Number of threads",
			REAL_OPTARGS, "threads",
			OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
			OPERAND, "input image file",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
			&opt_a,
			&opt_h,
			&opt_P,
			&operands,
			0
	};

	double          angle;		/* skew angle			 */
	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int				nthreads;	/* number of threads to use */

	/*
	 * begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	angle = 0.0;
	/*
	 * process options
	 */
	if (got_opt(opt_a)) {
		angle = real_arg(opt_a, 0);
		if (angle < SKEW_MIN || angle > SKEW_MAX) {
			error("skew angle must be >= %f and <= %f",
					SKEW_MIN, SKEW_MAX);
		}
	}
	else if (got_opt(opt_h)) {
		error("can't specify -h without -a");
	}

	/* check number of threads to use */
	if (got_opt(opt_P)) {
		nthreads = real_arg(opt_P, 0);
		if (nthreads > omp_get_max_threads()){
			nthreads = omp_get_max_threads();
			error("maximum number of threads is %i\n", omp_get_max_threads());
		}
	}
	else {
		nthreads = 1;
	}

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
	run_skew(fdi, got_opt(opt_a), angle, got_opt(opt_h), nthreads, fdo);

	/*
	 * end
	 */
	ipwexit(EXIT_SUCCESS);
}
