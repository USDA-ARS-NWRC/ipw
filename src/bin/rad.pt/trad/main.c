#include <math.h>

#include <unistd.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "radiation.h"

#define  CL_EMISS 0.998

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "reference elevation",
		REAL_OPTARGS, "elev",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "input vapor pressures in mb (default: Pa)",
	};

	static OPTION_T opt_c = {
		'c', "output cloud thermal radiation (emiss = 0.998)",
	};

	static OPTION_T opt_K = {
		'K', "input air temperatures in K (default: C)",
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_m,
		&opt_c,
		&opt_K,
		0
	};

	int	K;
	int	mb;
	int	ce;
	int	n;

	double	ea;
	double	emiss;
	double	cloud_lw_in;
	double	es;
	double	lw_in;
	double	press;
	double	ta;
	double	T0;
	double	z;

	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	z = real_arg(opt_z, 0);
	mb = got_opt(opt_m);
	ce = got_opt(opt_c);
	K  = got_opt(opt_K);

	/*	get input ta and ea	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "Input ta and vp:\n");

	n = 0;

	while (scanf("%lf %lf", &ta, &ea) != EOF) {

		/*	set ea to Pa if necessary	*/
		if (mb)
			ea *= 100.0;

		/*	set ta to K if necessary	*/
		if (!K)
			ta += FREEZE;

		/*	check for legal ea	*/
		es = sati(ta);
		if (es < ea) {
			if (ea > (es + 10.0))
				warn("ea=%g > es at ta; set to es at ta", ea);
			ea = es;
		}

		/*	calculate theoretical sea level atmos. emiss  */
		/*	from ta, ea, and z	*/
		
		emiss = brutsaert(ta, STD_LAPSE_M, ea, z, SEA_LEVEL);

		/*	calculate theoretical sea level air temp	*/

		T0 = ta - (z * STD_LAPSE_M);

		/*	calculate air pressure	*/

		press = HYSTAT(SEA_LEVEL, T0, STD_LAPSE,
			(z/1000.), GRAVITY, MOL_AIR);

		/*	calculate incoming lw rad	*/

		lw_in = (emiss * press/SEA_LEVEL) * STEF_BOLTZ *ta*ta*ta*ta;

		/* if -c is set, calculate incoming lw rad for cloudy conditions */
		if (!ce)
			printf("%.2f\n", lw_in);
		else {
			cloud_lw_in = CL_EMISS * STEF_BOLTZ *ta*ta*ta*ta;
			printf("%.2f\t%.2f\n", lw_in, cloud_lw_in);
		}

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
