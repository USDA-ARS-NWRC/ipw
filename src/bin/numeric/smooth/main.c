
#include <math.h>
#include "pgm.h"
#include "ipw.h"

#define	LARGE	100000

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_n = {
		'n', "running average or smoothing order",
		INT_OPTARGS, "order",
		REQUIRED, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		0
	};

	int	in;
	int	order;
	int	out;

	double	ave;
	double	sum;
	double	val;
	double	value[LARGE];

   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/***	PROCESS ARGS	***/

        order = int_arg(opt_n, 0);

	in = 0;
	out = 0;
	sum = 0.0;

	/***	SMOOTH VECTOR	***/

	while (scanf("%lf", &val) != EOF) {
		value[in] = val;
		sum += value[in];
		in++;
		if (in >= order) {
			ave = sum / order;
			printf("%g\n", ave);
			out++;
			sum -= value[in - order];
		}
	}

	fprintf(stderr, "%d input values\n", in);
	fprintf(stderr, "%d output values\n", out);

	ipwexit(EXIT_SUCCESS);
}
