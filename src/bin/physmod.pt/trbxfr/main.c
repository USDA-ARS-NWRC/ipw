#include	<unistd.h>
#include	<math.h>

#include	"ipw.h"
#include        "envphys.h"
#include        "pgm.h"

int
main(
		int	argc,
		char	**argv)
{
	static OPTION_T opt_z = {
			'z', "measurement site elevation (m)",
			REAL_OPTARGS, "elev",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_K = {
			'K', "input temperatures in K (default: C)",
	};

	static OPTION_T opt_m = {
			'm', "output water gain/loss in mm/m^2 s^-1",
	};

	static OPTION_T opt_d = {
			'd', "wind speed ht. (zu) & temp/humidity ht. (zt) are different",
	};

	static OPTION_T *optv[] = {
			&opt_z,
			&opt_K,
			&opt_m,
			&opt_d,
			0
	};

	int	dif;
	int	crt;
	int	K;
	int	mm;
	int	n;

	int	code;

	double	elev;
	double	z;
	double	z0;
	double	zt;
	double	zu;
	double	t;
	double	t0;
	double	e;
	double	e0;
	double	u;
	double	u0;
	double	pa;
	double	h;
	double	le;
	double	ev_air;


	/*	process optional args	*/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	if (got_opt(opt_z)) {
		elev = real_arg(opt_z, 0);
	}
	else {
		elev = 0.0;
	}

	/*	check K flag	*/

	K  = got_opt(opt_K);

	/*	check mm flag	*/

	mm = got_opt(opt_m);

	/*	check dif flag	*/

	dif = got_opt(opt_d);

	/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		if (dif)
			fprintf(stderr,
					"Input zu, zt, z0, t, t0, e, e0, u;\n");
		else
			fprintf(stderr,
					"Input z, z0, t, t0, e, e0, u, u0;\n");
	}

	/*	set pa from site elev	*/

	pa = HYSTAT (SEA_LEVEL, STD_AIRTMP, STD_LAPSE, (elev / 1000.0),
			GRAVITY, MOL_AIR);

	/*	read input data and do calculations	*/

	n = 0;

	if (dif) {
		while(scanf("%lf %lf %lf %lf %lf %lf %lf %lf",
				&zu, &zt, &z0, &t, &t0, &e, &e0, &u) == 8) {

			n++; 

			if (!K) {
				t  += FREEZE;
				t0 += FREEZE;
			}
			else
				if ((t < 0.0) || (t0 < 0.0))
					error("bad Kelvin temp. input");

			if ((code = hle1(pa, t, t0, zt, e, e0, zt, u, zu, z0,
					&h, &le, &ev_air)) != 0) {
				error("bad return code %d from hle1", code);
			}

			if (mm) {
				if (crt)
					printf("H=%.7f; LE=%.7f; EVAP=%e\n",
							h, le, ev_air);
				else
					printf("%.7f\t%.7f\t%e\n", h, le, ev_air);
			}
			else if (crt)
				printf("H=%.7f; LE=%.7f;\n", h, le);
			else
				printf("%.7f\t%.7f\n", h, le);
		}
	}

	else {
		while(scanf("%lf %lf %lf %lf %lf %lf %lf %lf",
				&z, &z0, &t, &t0, &e, &e0, &u, &u0) == 8) {

			n++; 

			if (!K) {
				t  += FREEZE;
				t0 += FREEZE;
			}
			else
				if ((t < 0.0) || (t0 < 0.0))
					error("bad Kelvin temp. input");

			if ((code = hle1(pa, t, t0, z, e, e0, z, u, z, z0,
					&h, &le, &ev_air)) != 0) {
				error("bad return code %d from hle1", code);
			}

			if (mm) {
				if (crt)
					printf("H=%.7f; LE=%.7f; EVAP=%e\n",
							h, le, ev_air);
				else
					printf("%.7f\t%.7f\t%e\n", h, le, ev_air);
			}
			else if (crt)
				printf("H=%.7f; LE=%.7f;\n", h, le);
			else
				printf("%.7f\t%.7f\n", h, le);
		}
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
