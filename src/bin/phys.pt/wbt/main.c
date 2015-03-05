#include	<unistd.h>

#include	"ipw.h"
#include 	"pgm.h"
#include        "envphys.h"

#define		RH_MAX  1.0
#define		RH_MIN  0.0
#define		VP_MAX  7500.0
#define		VP_MIN  0.0

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "elevation (m) for calculation of pa",
		REAL_OPTARGS, "elev",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_p = {
		'p', "air pressure (Pa) (default: 1 atmos)",
		REAL_OPTARGS, "pressure",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "also output dewpoint temp. in C",
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_p,
		&opt_d,
		0
	};

	int	n;
	int	dp;

	double	airpress;
	double	d_point = 0.0;
	double	tdry;
	double	twet;
	double	rh;
	double	vpress;
	double	z;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* see if more args */

        if (got_opt(opt_z)) {
                z = real_arg(opt_z, 0);
        }
        else {
                z = 0.0;
        }

        if (got_opt(opt_p)) {
                airpress = real_arg(opt_p, 0);
        }
        else {
		if (z == 0.0) {
			airpress = SEA_LEVEL;
		}
		else {
			airpress = HYSTAT (SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
				   (z / 1000.0), GRAVITY, MOL_AIR);
		}
	}

	dp = got_opt(opt_d);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO))
		fprintf(stderr,"Input Ta and RH pair;\n");

	/*	read input data and do calculations  	*/

	n = 0;

	while (scanf("%lf %lf", &tdry, &rh) == 2) {
		/*      check input humidity    */
		if ((rh < RH_MIN) || (rh > RH_MAX))
			error("input RH (%lf) out of range", rh);		
		n++;

		/*	convert dry temps to Kelvin	*/
		tdry += FREEZE;

		/*	calculate vapor pressure	*/
		vpress = sati(tdry) * rh;

		d_point = dew_point(vpress);

		/* call wetbulb function */
		twet = wetbulb(tdry, d_point, airpress);

		/*	output results to stdout	*/
		if(dp) {
			printf("%6.2f %6.2f\n", (twet - FREEZE), (d_point - FREEZE));
		} else {
			printf("%6.2f\n", (twet - FREEZE));
		}
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
