#include        <unistd.h>

#include	"ipw.h"
#include        "envphys.h"
#include	"radiation.h"

#include	"pgm.h"

#define		TB	288.0

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "elevation (default: 0.0 m)",
		REAL_OPTARGS, "elev",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_p = {
		'p',  "air pressure (default 101324.6 Pa)",
		REAL_OPTARGS, "pressure",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_v = {
		'v', "view factor (default 1.0)",
		REAL_OPTARGS, "view-factor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "temperature lapse rate (default -0.0065 K/m)",
		REAL_OPTARGS, "lapse-rate",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_t = {
		't', "constant timestep (decimal hours)",
		REAL_OPTARGS, "timestep",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_p,
		&opt_v,
		&opt_l,
		&opt_t,
		0
	};

	short	crt;
	short	n;

	double	advec;
	double	beta;
	double	ea;
	double	emiss;
	double	es;
	double	inet;
	double	lapse;
	double	netrad;
	double	pa;
	double	rate;
	double	snet;
	double	store;
	double	ta;
	double	rtime;
	double	total;
	double	ts;
	double	tstep;
	double	vf;
	double	z;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check for optional args	*/

	if (got_opt(opt_z)) {
		z = real_arg(opt_z, 0);
	}
	else {
		z = 0.0;
	}

	if (got_opt(opt_l)) {
		lapse = real_arg(opt_l, 0);
	}
	else {
		lapse = -0.0065;
	}

	if (got_opt(opt_p)) {
		pa = real_arg(opt_p, 0);
		if (z == 0.0) {
			z = INV_HYSTAT (SEA_LEVEL, TB, 0.0, pa, TB,
					GRAVITY, MOL_AIR);
		}
	}
	else {
		if (z == 0.0) {
			pa = SEA_LEVEL;
		}
		else {
			pa = HYSTAT (SEA_LEVEL, TB,
				     (lapse * 1000.0), (z / 1000.0),
				     GRAVITY, MOL_AIR);
		}
	}

	if (got_opt(opt_v)) {
		vf = real_arg(opt_v, 0);
	}
	else {
		vf = 1.0;
	}

	if (got_opt(opt_t)) {
		tstep = real_arg(opt_t, 0) * 3600;
	}
	else {
		tstep = 1.0;
	}

/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		crt = TRUE;
		if (tstep == 1.0)
			fprintf(stderr,
			   "Input ts, ta, ea, snet, advec, store, time;\n");
		else
			fprintf(stderr,
			   "Input ts, ta, ea, snet, advec, store;\n");
	}
	else
		crt = FALSE;

/*	read input data and do calculations	*/

	n = 0;
	emiss = 1.0;

	if (tstep == 1.0) {
		while(scanf("%lf %lf %lf %lf %lf %lf %lf",
		   &ts, &ta, &ea, &snet, &advec, &store, &rtime) == 7) {

			n++; 

			ts += FREEZE;
			ta += FREEZE;
			rtime *= 3600.0;
			if (rtime == 0.0)
				rtime += 1.0;
			es = satw(ts);
			inet = net_therm(ta, ts, pa, ea, emiss,
					vf, lapse, z);
			netrad = inet + snet;
			beta = bowen(pa, ta, ts, ea, es);
			rate = bevap(netrad, advec, beta, store, ta);
			total = rate * rtime;

			if (crt)
				printf("rate = %e (mm/sec); total = %8.5f (mm);\n",
					rate, total);
			else
				printf("%8.5f\n", total);
		}
	}

	else {
		while(scanf("%lf %lf %lf %lf %lf %lf",
			&ts, &ta, &ea, &snet, &advec, &store) == 6) {

			n++;

			ts += FREEZE;
			ta += FREEZE;
			es = satw(ts);
			inet = net_therm(ta, ts, pa, ea, emiss,
					vf, lapse, z);
			netrad = inet + snet;
			beta = bowen(pa, ta, ts, ea, es);
			rate = bevap(netrad, advec, beta, store, ta);
			total = rate * tstep;

			if (crt)
				printf("rate = %e; total = %8.5f;\n",
					rate, total);
			else
				printf("%8.5f\n", total);
		}
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
