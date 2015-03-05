#include <errno.h>

#include "pgm.h"
#include "ipw.h"

#define	NMAX	50000

static	int	n_x;

static	double	half_r;
static	double	cx_x[NMAX];
static	double	x_x[NMAX];
static	double	y_x[NMAX];

/* ----------------------------------------------------------------------- */

double
halfint(
	double	x)
{
	double	hr;

	hr = splint (x_x, y_x, n_x, cx_x, x_x[0], x);

	if (errno == ERROR)
		error("error calculating integral of spline function");

	return (hr - half_r);
}

/* ----------------------------------------------------------------------- */

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_a = {
		'a', "lower integration limit (default: first x value)",
		REAL_OPTARGS, "lower",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "upper integration limit (default: last x value)",
		REAL_OPTARGS, "upper",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_y = {
		'y', "Read only y values, and compute x values",
	};

	static OPTION_T opt_d = {
		'd', "delta-x value for computing x values (default: 1.0)",
		REAL_OPTARGS, "dx",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_r = {
		'r', "remove background value (1st y value) from y values",
	};

	static OPTION_T *optv[] = {
		&opt_a,
		&opt_b,
		&opt_y,
		&opt_d,
		&opt_r,
		0
	};

	int	j;
	int	n;
	int	nox;
	int	rb;

	double	a;
	double	b;
	double	x;
	double	temp;
	double	bkgd;
	double	dx;
	double	result;
	double	z;

	double	halfint();



	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

        if (got_opt(opt_a)) {
                a = real_arg(opt_a, 0);
        }
        else {
		a = 0.0;
        }

        if (got_opt(opt_b)) {
                b = real_arg(opt_b, 0);
        }
        else {
		b = 0.0;
        }

	nox = got_opt(opt_y);

        if (got_opt(opt_d)) {
		if (! nox)
			error("-d option must be used with -y option");
                dx = real_arg(opt_d, 0);
        }
        else {
		dx = 1.0;
        }

	rb  = got_opt(opt_r);


	/*	read input file	*/

	n = 0;

	if (nox) {
		for (n = 0, x = a; n < NMAX; n++, x += dx) {
			if (scanf("%lf", &y_x[n]) == EOF)
				break;
			x_x[n] = x;
		}
	}
	else {
		for (n = 0; n < NMAX; n++) {
			if (scanf("%lf %lf", &x_x[n], &y_x[n]) == EOF)
				break;
		}
	}
	if (n >= NMAX) {
		if (scanf("%lf", &temp) != EOF)
			error("too many input values (more than %d)", NMAX);
	}


	n_x = n;

	if (n <= 0)
		error ("no input values");

	/*	set limits of integration	*/

	if (! got_opt(opt_a))
		a = x_x[0];
	
	if (! got_opt(opt_b))
		b = x_x[n-1];

	/*	remove background?	*/

	if (rb) {
		bkgd = y_x[0];
		for (j = 0; j < n; j++)
			y_x[j] -= bkgd;
	}

	/*	calculate spline coefficients	*/

	if (akcoef(x_x, y_x, n, cx_x) == ERROR)
	    error("error calculating Akima spline coefficients");

	/*	calculate the integral from a to b	*/

	result = splint(x_x,y_x,n,cx_x,a,b);

	if (errno == ERROR)
		error("error calculating integral of spline function");

	/*	calculate x-value at half integral	*/

	half_r = result / 2.0;

	z = zerobr(x_x[0], x_x[n-1], 1.e-5, halfint);

	if (errno)
		error("error finding x-value at half-integral");

	printf ("no. values = %d,  integral = %g, x at half int = %g\n",
		 n, result, z);

	ipwexit (EXIT_SUCCESS);
}
