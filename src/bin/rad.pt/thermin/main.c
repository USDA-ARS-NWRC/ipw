#include <math.h>

#include "ipw.h"
#include "envphys.h"
#include "radiation.h"
#include "pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_a = {
		'a', "reference air temperature in K",
		REAL_OPTARGS, "air-temp",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "reference dew point temperature in K",
		REAL_OPTARGS, "dewpoint-temp",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_z = {
		'z', "reference elevation in m",
		REAL_OPTARGS, "elev",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "elevation step in m",
		REAL_OPTARGS, "elev-step",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "minimum elevation in m",
		REAL_OPTARGS, "elev-min",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_x = {
		'x', "maximum elevation in m",
		REAL_OPTARGS, "elev-max",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_a,
		&opt_d,
		&opt_z,
		&opt_s,
		&opt_m,
		&opt_x,
		0
	};

	double	ta;		/*	air temp	*/
	double	tw;		/*	dew point	*/
	double	z;		/*	ref. elev	*/
	long	zstep;		/*	elev step	*/
	long	zmin;		/*	min elev	*/
	long	zmax;		/*	max elev	*/

	int	j;
	double	ea;
	double	emiss;
	double	lw_in;
	double	press;
	double	T;
	double	T0;


	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	ta = real_arg(opt_a, 0);
	tw = real_arg(opt_d, 0);
	z = real_arg(opt_z, 0);

        if (got_opt(opt_s)) {
                zstep = real_arg(opt_s, 0);
        }
        else {
                zstep = 500;
        }

        if (got_opt(opt_m)) {
                zmin = real_arg(opt_m, 0);
        }
        else {
                zmin = 0;
        }

        if (got_opt(opt_x)) {
                zmax = real_arg(opt_x, 0);
        }
        else {
		zmax = 5000;
        }

	/*	calculate theoretical sea level atmos. emissivity  */
	/*	from reference level ta, tw, and z	*/
	
	if(tw > ta) {
		tw = ta;
		warn("tw=%g > ta; tw set to ta",tw);
	}

	ea = sati(tw);
	emiss = brutsaert(ta, STD_LAPSE_M, ea, z, SEA_LEVEL);

	/*	calculate theoretical sea level air temp	*/

	T0 = ta - (z * STD_LAPSE_M);

	/*	calculate incoming lw rad for each level	*/

	j = zmin;
	while( j <= zmax) {

		T = T0 + (j * STD_LAPSE_M);
		press = HYSTAT(SEA_LEVEL, T0, STD_LAPSE,
			(j/1000.), GRAVITY, MOL_AIR);
		lw_in = (emiss * press/SEA_LEVEL) * STEF_BOLTZ *T*T*T*T;

		printf("%8d %8.2f\n", j, lw_in);

		j += zstep;

	}

	ipwexit(EXIT_SUCCESS);
}
