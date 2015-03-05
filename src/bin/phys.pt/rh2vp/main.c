#include	<unistd.h>

#include	"ipw.h"
#include        "pgm.h"
#include	"envphys.h"

#define		RH_MAX	1.0
#define		RH_MIN	0.0
#define		VP_MAX	7500.0
#define		VP_MIN	0.0

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_i = {
		'i', "inverse: RH from Ta and vp"
	};

	static OPTION_T opt_c = {
		'c', "clip output RH to <=100, or vp <= svp"
	};

	static OPTION_T opt_F = {
		'F', "input Ta in F (default: C)",
	};

	static OPTION_T opt_m = {
		'm', "output vp in mb (default: Pa)",
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_c,
		&opt_F,
		&opt_m,
		0
	};

	bool_t	inverse;	/* invert vp to RH	*/
	bool_t	clip;		/* clipping RH or vp	*/
	int	F;		/* air temp if F	*/
	int	mb;		/* output vp in mp	*/
	int	n;		/* input counter	*/

	double	air_temp;
	double	humidity;
	double	vpress;
	double	satvpress;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* see if more args */

	inverse = got_opt(opt_i);
	clip 	= got_opt(opt_c);
	F	= got_opt(opt_F);
	mb	= got_opt(opt_m);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		if (inverse)
			fprintf(stderr,"Input air temp and vp;\n");
		else
			fprintf(stderr,"Input air temp and RH;\n");
	}

	/*	read input data and do calculations  	*/

	n = 0;
	if (inverse) {
		/* calculate RH from temperature and vp */

		while (scanf("%lf %lf", &air_temp, &vpress) == 2) {

			/*	check input vpress	*/
			if (vpress < VP_MIN)
				error("input vp (%lf) < 0.0", vpress);

			n++;

			/*	convert dew point temp to C if in F	*/
			if(F)
				air_temp = (air_temp - 32.0) * (5.0/9.0);

			/*	convert air_temp to K	*/
			air_temp += FREEZE;

			/*	calculate sat_vp	*/
			satvpress = sati(air_temp);

			if (clip) {
				if (vpress > satvpress)
					vpress = satvpress;
			} else {
				if (vpress > satvpress)
					error("vp (%lf) > sat_vp (%lf)", vpress,
			      			satvpress);
			}

			humidity = vpress / sati(air_temp);

			/* output RH to stdout */

			printf("%4.3f\n", humidity);
		}
	} else	{
		/* calculate vapor pressure from air temp and RH */

		while (scanf("%lf %lf", &air_temp, &humidity) == 2) {

			/*	check input humidity	*/
			if (humidity < RH_MIN)
				error("input RH (%lf) < 0.0", vpress);

			n++;

			/*	convert dew point temp to C if in F	*/
			if(F)
				air_temp = (air_temp - 32.0) * (5.0/9.0);

			/*	convert air_temp to K	*/
			air_temp += FREEZE;

			if (clip) {
				if (humidity > RH_MAX)
					humidity = RH_MAX;
			} else {
				if (humidity > RH_MAX)
					error("RH (%lf) > 1.0", humidity);
			}
				
			vpress = sati(air_temp);

			/* calculate vapor pressure */

			vpress *= humidity;

			/*	output results to stdout	*/

			if(mb)
				printf("%6.2f\n", vpress/100.0);
			else
				printf("%6.2f\n", vpress);
		}
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
