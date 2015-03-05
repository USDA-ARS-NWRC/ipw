#include	<unistd.h>

#include	"ipw.h"
#include 	"pgm.h"
#include        "envphys.h"


int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};

	int	n;

	double ta;	/* air temperature */
	double slope1;	/* gradient slope of saturation vp-temperature curve, Pa/K */
	double slope2;	/* C-C slope of saturation vp-temperature curve, Pa/K */
	double t1;	/* temperature 1 for gradient slope */
	double t2;	/* temperature 2 for gradient slope */
	double R_air;	/* gas constant for air */
	double xlhv;	/* latent heat of vaporization at ta */
	double xlhf;	/* latent heat of fusion at ta */
	double xlh;	/* latent heat at ta */
	double sati();	/* returns saturation vapor pressure, Pa, at given temp(k) */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		fprintf(stderr,"Input air temperature (C);\n");
	}

	/*	read input data and do calculations  	*/

	n = 0;
	printf("Ta\tgrad\tC-C\n");

	while (scanf("%lf", &ta) == 1) {

		n++;

	/* convert temperature to kelvin */

		ta += FREEZE;
		t1 = ta + 0.5;
		t2 = ta - 0.5;

	/* calculate gas constant for air */

		R_air = (RGAS)/(MOL_AIR);

	/* find latent heat of vaporization, or vaporization + fusion */
		if (ta <= FREEZE) {
			xlhv = LH_VAP(ta);
			xlhf = LH_FUS(ta);
			xlh  = xlhv + xlhf;
		}
		else
			xlh = LH_VAP(ta);

	/* calculate slope of saturation vapor pressure/temperature curve */

		slope1 = (sati(t1) - sati(t2));

	/* Clausius-Clapeyron equation for slope of sat. vap/temp curve */

		slope2 = (0.622 * xlh * sati(ta)) / (R_air * ta * ta);

		printf("%4.1f\t%5.2f\t%5.2f\n", (ta-FREEZE), slope1, slope2);
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
