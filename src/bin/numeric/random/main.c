#include <time.h>
#include <stdlib.h>

#include "ipw.h"
#include "pgm.h"

static int      prec = 0;		/* output precision		 */

static int
dprint(double x)
{
	return (printf("%d ", (int) (x < 0 ? x - 0.5 : x + 0.5)));
}

static int
gprint(double x)
{
	return (printf("%.*g ", prec, x));
}

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_r = {
		'r', "range of values: one min,max pair per column",
		REAL_OPTARGS, "m",
		REQUIRED, 2
	};

	static OPTION_T opt_n = {
		'n', "# of rows to generate",
		INT_OPTARGS, "n",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_p = {
		'p', "output precision, 0 == integer (default)",
		INT_OPTARGS, "precision",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's',
		"random generator seed, default is to get seed from clock",
		INT_OPTARGS, "seed",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&opt_r,
		&opt_p,
		&opt_s,
		0
	};

	REG_4 int       col;		/* current column		 */
	REG_1 double   *min;		/* -> min value [column]	 */
	REG_5 int       ncols2;		/* 2 * # columns to print	 */
	REG_6 int       nrows;		/* # rows to print		 */
	REG_2 double   *range;		/* -> range of values [column]	 */
	REG_3 int       (*rprint) ();	/* -> output function		 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	nrows = int_arg(opt_n, 0);
	if (nrows < 1) {
		error("# of values must be > 0");
	}

	ncols2 = n_args(opt_r);
	if (ncols2 & 1) {
		error("range(s) must be specified by min,max pairs");
	}

	if (got_opt(opt_p)) {
		prec = int_arg(opt_p, 0);
		if (prec < 0) {
			error("output precision must be >= 0");
		}
	}

	rprint = (prec > 0) ? gprint : dprint;
 /*
  * The output columns are numbered 0, 2, 4, ...  This allows us to set
  * 2 pointers into the min,max array, then use the column numbers as
  * indices on these pointers.
  */
	min = real_argp(opt_r);
	range = min + 1;
 /*
  * convert max to range
  */
	for (col = 0; col < ncols2; col += 2) {
		range[col] -= min[col];
	}
 /*
  * generate random numbers
  */
 /* seed from command line	 */
	if (got_opt(opt_s)) {
		srand48((long) int_arg(opt_s, 0));
	}
 /* generate seed from time of day	 */
	else {
		srand48((long) time(NULL));
	}

	do {
		for (col = 0; col < ncols2; col += 2) {
		       (void) ((*rprint) (drand48() * range[col] + min[col]));
		}

		putchar('\n');
	} while (--nrows > 0);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
