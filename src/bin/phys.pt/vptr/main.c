#define	BCON	1.24		/* Brutsaert's constant */

#include <math.h>
#include <unistd.h>

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"
#include "radiation.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "elevation (m)",
		REAL_OPTARGS, "elev",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_K = {
		'K', "input air temperatures in K (default: C)",
	};

	static OPTION_T opt_m = {
		'm', "output vapor pressures in mb (default: Pa)",
	};

	static OPTION_T opt_r = {
		'r', "output both vapor pressure and relative humidity",
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_K,
		&opt_m,
		&opt_r,
		0
	};

	int	K;
	int	mb;
	int	RH;
	int	n;

	double	atmos;
	double	ea;
	double	emiss;
	double	es;
	double	E0;
	double	E0s;
	double	lw_in;
	double	lw_max;
	double	press;
	double	rh;
	double	ta;
	double	T0;
	double	z;


	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	z = real_arg(opt_z, 0);
	K  = got_opt(opt_K);
	mb = got_opt(opt_m);
	RH = got_opt(opt_r);

	/*	get input ta and lw_in	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "Input ta and lw_in:\n");

	n = 0;

	while (scanf("%lf %lf", &ta, &lw_in) != EOF) {

		/*	set ta to K if necessary	*/
		if (!K)
			ta += FREEZE;

		/*	check input thermal radiation value	*/

		lw_max = STEF_BOLTZ * ta * ta * ta * ta;

		if (lw_in > lw_max) {
			lw_in = lw_max;
			warn("lw_in > max possible; set to max");
		}

		/*	calculate theoretical sea level air temp	*/

		T0 = ta - (z * STD_LAPSE_M);

		/*	calculate air pressure	*/

		press = HYSTAT(SEA_LEVEL, T0, STD_LAPSE,
			(z/1000.), GRAVITY, MOL_AIR);

		/*	calculate atmospheric thickness	*/

		atmos = press / SEA_LEVEL;

		/*	calcualte atmospheric emissivity	*/

		emiss = (lw_in / (STEF_BOLTZ * ta * ta * ta * ta));

		/*	calculate theoretical sea level vapor press	*/

		/*	in Pa (Brutsaert eq is for mb...)	*/
		E0 = 100.0 * T0 * pow((emiss / (atmos * BCON)), 7.0);

		/*	set sat vp for sea level	*/

		E0s = sati(T0);

		/*	check for legal E0, and set rh	*/
		if (E0 > E0s) {
			E0 = E0s;
			rh = 1.0;
			/* warn("E0 > E0s; set to E0s");*/
		}
		else
			rh = E0 / E0s;

		/*	calculate es and ea	*/

		es = sati(ta);
		ea = rh * es;

		/*	set ea to mb if necessary	*/
		if (mb)
			ea /= 100.0;

		if (isatty(STDIN_FILENO)) {
			/***	check everything	***/
			printf("ta = %.4f\n", ta);
			printf("lw_in = %.4f\n", lw_in);
			printf("emiss = %.4f\n", emiss);
			printf("press = %.4f\n", press);
			printf("T0 = %.4f\n", T0);
			printf("E0 = %.4f\n", E0);
			printf("E0s = %.4f\n", E0s);
			printf("rh = %.4f\n", rh);
			printf("es = %.4f\n", es);
			printf("ea = %.4f\n", ea);
		}
		else if (RH)
			printf("%.2f\t%.2f\n", ea, rh);
		else
			printf("%.2f\n", ea);

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
