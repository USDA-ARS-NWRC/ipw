
#include	"ipw.h"
#include	"pgm.h"

#define	NMAX	40960

int
main(
		int	argc,
		char	**argv)
{
	static OPTION_T opt_d = {
		'd', "desired x interval",
		REAL_OPTARGS, "delta-x",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "staring x value",
		REAL_OPTARGS, "first-x",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "number of x intervals",
		INT_OPTARGS, "count",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_d,
		&opt_s,
		&opt_n,
		0
	};

	int	i;
	int	nin;
	int	nout;
	int	nx;

	double	dx;
	double	x0;
	double	temp;
	double	*xin;
	double	*xout;
	double	*yin;
	double	*yout;
	double	*coef;


   /* get args */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	dx = real_arg(opt_d, 0);

	/*	 set storage for xin and yin	*/

	if ((xin = (double *)(ecalloc(NMAX, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of x values");
	 }

	if ((yin = (double *)(ecalloc(NMAX, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of y values");
	 }

	/*	read input x-y pairs	*/

	for (nin = 0; nin < NMAX; nin++) {
		if (scanf("%lf %lf", &xin[nin], &yin[nin]) == EOF)
			break;
	}
	if (nin >= NMAX) {
		if (scanf("%lf", &temp) != EOF)
			error("too many x-y pairs (more than %d)", NMAX);
	}

	/*	release unused space in xin and yin	*/

	if ((xin = (double *)(realloc(xin, nin * sizeof(double)))) == NULL) {
		error("cannot release unused space in array of x values");
	}

	if ((yin = (double *)(realloc(yin, nin * sizeof(double)))) == NULL) {
		error("cannot release unused space in array of y values");
	}

	/*	set x0 and nout	*/

        if (got_opt(opt_s)) {
                x0 = real_arg(opt_s, 0);
        }
        else {
		x0 = xin[0];
        }

	if ((x0 < xin[0]) || (x0 > xin[nin-1]))
		error("x0=%f; outside input x range", x0);

	nout = 1 + (xin[nin-1] - x0) / dx;

        if (got_opt(opt_n)) {
                nx = int_arg(opt_n, 0);
		if (nout > nx)
			nout = nx;
        }


	/*	allocate space for output vectors	*/

	if ((xout = (double *)(ecalloc(nout, sizeof(double)))) == NULL) {
		error("cannot allocate space for output array of x values");
	 }

	if ((yout = (double *)(ecalloc(nout, sizeof(double)))) == NULL) {
		error("cannot allocate space for output array of y values");
	 }

	if ((coef = (double *)(ecalloc(3 * nin, sizeof(double)))) == NULL) {
		error("cannot allocate space for output array of coefficients");
	 }

	/*	set output x array	*/

	for(i=0; i<nout; i++) {
		xout[i] = (dx * i) + x0;
	}

	/***	calculate akima spline coefs	***/

	if (akcoef(xin, yin, nin, coef) == ERROR)
		error("cannot calculate Akima spline coefficients");

	/***	evaluate spline coefs	***/

	if (seval(xin, yin, nin, coef, xout, yout, nout) == ERROR)
		error("cannot evaluate coefficients");

	/*	print result on stdout	*/

	for(i=0; i<nout; i++) {
		printf ("%.5f %.5f\n", xout[i], yout[i]);
	}

	ipwexit (EXIT_SUCCESS);
}
