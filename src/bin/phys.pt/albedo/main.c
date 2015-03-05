#define MAXV	1.0		/* vis albedo when gsize = 0 */
#define MAXIR	0.85447		/* IR albedo when gsize = 0 */
#define IRFAC	-0.02123 	/* IR decay factor */
#define VFAC	500.0		/* visible decay factor */
#define VZRG	1.375e-3	/* vis zenith increase range factor */
#define IRZRG	2.0e-3		/* ir zenith increase range factor */
#define IRZ0	0.1		/* ir zenith increase range, gsize=0 */

#include	<stdio.h>
#include	<unistd.h>

#include	<math.h>

#include 	"ipw.h"
#include	"pgm.h"

double growth(double t);

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

	static OPTION_T opt_g = {
		'g', "effective grain radius of last storm (mu m)",
		REAL_OPTARGS, "gsize",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "maximum grain radius allowed (mu m)",
		REAL_OPTARGS, "maxgsz",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "contamination factor for visible albedo",
		REAL_OPTARGS, "dirt",
		REQUIRED, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_s,
		&opt_g,
		&opt_m,
		&opt_d,
		0
	};

	int	n;		/* record counter */

	double	alb_ir;		/* IR albedo */
	double	alb_v;		/* Visible albedo */
	double	cosz;		/* cosine of zenith angle */
	double	day;		/* decimal days since last storm */
	double	dirt;		/* visible contamination factor */
	double	dzir;		/* ir diurnal increase range */
	double	dzv;		/* visible diurnal increase range */
	double	gir;		/* sqrt of effective ir grain radius */
	double	gv;		/* sqrt of effective vis grain radius */
	double	growth_factor;	/* grain growth factor */
	double	gsize;		/* initial grain radius */
	double	maxgsz;		/* grain radius growth limit */
	double	radius_ir;	/* sqrt initial eff ir grain rad */
	double	radius_v;	/* sqrt initial eff vis grain rad */
	double	range_ir;	/* range of sqrt eff ir grain rad */
	double	range_v;	/* range of sqrt eff vis grain rad */
	double	start;		/* day of last storm */


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/***	PROCESS ARGS	***/

	/* opt_check(4, 4, 4, &opt_s, &opt_g, &opt_m, &opt_d); */

	start = real_arg(opt_s, 0);
	gsize = real_arg(opt_g, 0);
	maxgsz = real_arg(opt_m, 0);
	dirt = real_arg(opt_d, 0);

	/* check inputs */
	if (gsize <=0 || gsize >= 500)
		error("unrealistic input: gsize=%lg", gsize);

	if (maxgsz <= gsize || maxgsz >= 2000)
		error("unrealistic input: maxgsz=%lg", maxgsz);

	if (dirt <= 1.0 || dirt >=10)
		error("unrealistic input: dirt=%lg", dirt);

	/*	set initial grain radii for vis and ir	*/
	radius_ir = sqrt(gsize);
	range_ir = sqrt(maxgsz) - radius_ir;
	radius_v = sqrt(dirt * gsize);
	range_v = sqrt(dirt * maxgsz) -radius_v;

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		printf("Input time (days) and cosz:\n");

	n = 0;

	/*	read input data	*/
	while (scanf("%lf %lf", &day, &cosz) != EOF) {

		/* check input time */
		if (day < start)
			error("input time, %lg, before start", day);

		/* check to see if sun is up */
		if (cosz > 0.0) {
			/* calc grain growth decay factor */
			growth_factor = growth((day - start) + 1.0);

			/* calc effective gsizes for vis & ir */
			gv = radius_v + (range_v * growth_factor);
			gir = radius_ir + (range_ir * growth_factor);

			/* calc albedos for cos(z)=1 */
			alb_v = MAXV - (gv / VFAC);
			alb_ir = MAXIR * exp(IRFAC * gir);

			/* calculate effect of cos(z)<1 */

			/* adjust diurnal increase range */
			dzv = gv * VZRG;
			dzir = (gir * IRZRG) + IRZ0;

			/* calculate albedo */
			alb_v += dzv * (1.0 - cosz);
			alb_ir += dzir * (1.0 - cosz);
		}
		else {
			alb_v = 0.0;
			alb_ir = 0.0;
		}

		/* write result to stdout */

		printf("%.4f\t%.4f\n", alb_v, alb_ir);

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}

/*
** name
**	growth - estimate snow grain growth as a function of time
**
** synopsis
**	double growth(t)
**		double t;	time since last snow storm
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
**	fits measured values, but has no real physical basis...
**
** history
**	June 1987:	written by D. Marks, CSL, UCSB;
*/

double
growth(
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
