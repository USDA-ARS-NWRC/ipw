#include <unistd.h>

#define DKR	0.18	/* default range of albedo decay with time */
#define DIR	0.18	/* range of diurnal albedo increase */
#define MXA	0.82	/* default max albedo when cosz is 1.0 */

#include "ipw.h"
#include "pgm.h"

double	decay(double time);
double	diurnal(double cosz);

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_s = {
		's', "time of last snow storm (decimal days)",
		REAL_OPTARGS, "start",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "maximum albedo when cosz=1.0 at that time",
		REAL_OPTARGS, "max-albedo",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "range of albedo decay with time",
		REAL_OPTARGS, "decay-range",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_s,
		&opt_n,
		&opt_d,
		0
	};

	int	n;

	double	alb_1;
	double	alb_t;
	double	day;
	double	decay_factor;
	double	dir;
	double	diurnal_factor;
	double	dkr;
	double	cosz;
	double	max_alb;
	double	start;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/***	PROCESS ARG	***/

	start = real_arg(opt_s, 0);

	if (got_opt(opt_n)) {
		max_alb = real_arg(opt_n, 0);

		/*	check input albedo	*/
		if (max_alb > 1.0) {
			error("max-albedo > 1.0, %g", max_alb);
		}
	}
	else {
		max_alb = MXA;
	}

	if (got_opt(opt_n)) {
		dkr = real_arg(opt_n, 0);
	}
	else {
		dkr = DKR;
	}

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "Input time and cosz:\n");

	n = 0;

	/*	read input data	*/
	while (scanf("%lf %lf", &day, &cosz) != EOF) {

		/* check input time */
		if (day < start)
			error("input time, %g, before start", day);

		/* check to see if sun is up */
		if (cosz > 0.0) {
			/* calculate time decay */
			decay_factor = decay((day - start) + 1.0);

			alb_1 = max_alb - (dkr * decay_factor);

			/* calculate diurnal increase */
			diurnal_factor = diurnal(cosz);

			/* adjust diurnal increase range (dir) */
			if (alb_1 <= MXA)
				dir = DIR;
			else
				dir = 1.0 - alb_1;

			/* calculate albedo */
			alb_t = alb_1 + (dir * diurnal_factor);
		}
		else
			alb_t = 0.0;

		/* write result to stdout */

		printf("%.4f\n", alb_t);

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}

/*
** name
**	decay - estimate snow albedo decay as a function of time
**
** synopsis
**	double decay(t)
**		double t;	time since initiation of decay fn
**
** description
**	Second order Chebyshev polynomial, with coefs. 4,3,2,1;
**		factor = ((a + bt + t**2) / (c + dt + t**2)) - 1.0
**	(see: Acton, 1970, pgs. 292-302)
**	Function ranges from 1.0-0.0 while t ranges from 1.0-infinity;
**	80% of range has been achieved by t=9;
**	Function returns (1.0 - factor);
**
** bugs
**	appears to work ok, but has no real physical basis...
**
** history
**	June 1987:	written by D. Marks, CSL, UCSB;
*/

double
decay(
	double	t)
{
	double	a;
	double	b;
	double	c;
	double	d;
	double	factor;

	a = 4.0;
	b = 3.0;
	c = 2.0;
	d = 1.0;

	factor = (a+(b*t)+(t*t))/(c+(d*t)+(t*t)) - 1.0;

	return(1.0 - factor);
}

/*
** name
**	diurnal - estimate change of snow albedo as a function
**			of increased solar zenith angle
** synopsis
**	double diurnal(cz)
**		double	cz;	cosine of solar zenith
**
** description
**	Returns factor (1.0 - cos(z)) to approximate the increase
**	in measured reflectance with solar zenith;
**	Generates curve that fits measured data well, and agrees
**	generally with work by Choudhury (1981),
**	and McGuffie and Henderson-Sellers (1985)
**
** bugs
**	appears to work ok, but has no real physical basis...
**
** history
**	June 1987:	written by D. Marks, CSL, UCSB;
*/

double
diurnal(
	double	cz)
{
	double	factor;

	factor = 1.0 - cz;
	return(factor);
}
