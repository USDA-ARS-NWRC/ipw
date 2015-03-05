#include	<unistd.h>

#include	"ipw.h"
#include        "pgm.h"
#include	"envphys.h"

#define		SP_MIN	0.0
#define		VP_MIN	0.0

int
main(
	int	argc,
	char	**argv)
{

	static OPTION_T opt_i = {
		'i', "inverse: sp_hum from vp"
	};

	static OPTION_T *optv[] = {
		&opt_i,
		0
	};

	int	n;		/* input counter	*/
	int	inverse;	/* use inverse func	*/

	double	sp_hum;
	double	Pa;
	double	vpress;

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* see if more args */

	inverse = got_opt(opt_i);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		if (inverse)
			fprintf(stderr,"Input specific humidity and air pressure;\n");
		else
			fprintf(stderr,"Input vapor pressure and air pressure;\n");
	}

	/*	read input data and do calculations  	*/

	n = 0;
	if (!inverse) {
		/* calculate specific humidity from vapor pressure */

		while (scanf("%lf %lf", &vpress, &Pa) == 2) {

			/*	check input vpress	*/
			if (vpress < VP_MIN)
				error("input VP (%lf) < 0.0", vpress);

			n++;
			
			/* calculate specific humidity (in gm/kg)	*/

			sp_hum = SPEC_HUM(vpress, Pa) * 1000.0;

			/*	output results to stdout	*/

			printf("%8.4f\n", sp_hum);
		}
	} else	{
		/* calculate vp from sp_hum */

		while (scanf("%lf %lf", &sp_hum, &Pa) == 2) {

			/*	check input sp_hum	*/
			if (sp_hum < SP_MIN)
				error("input sp_hum (%lf) < 0.0", sp_hum);

			n++;

			/* convert sp_hum to kg/kg */

			sp_hum /= 1000.0;

			/*	calculate vp	*/

			vpress = INV_SPEC_HUM(sp_hum, Pa); 

			/* output vp to stdout */

			printf("%8.2f\n", vpress);
		}
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
