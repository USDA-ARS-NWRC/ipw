#include	<unistd.h>

#include	"ipw.h"
#include	"pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};

	int	crt;
	int	n;

	double  precip;
	double  t_precip;
	double	percent_snow;
	double	rho_snow;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		fprintf(stderr,
		   "Input precip (kg (mm H2O)), t_precip (C);\n");
	}

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf",
                     &precip, &t_precip) == 2) {

		n++; 

		calc_snow(t_precip, &percent_snow, &rho_snow);

		if (crt) {
			printf("Precip_Mass = %.1f Percent_Snow = %.2f Snow_Density = %.3f Precip_Temp = %.1f\n",
                                precip, percent_snow, rho_snow, t_precip);
		}
		else {
			printf("%.1f\t%.2f\t%.3f\t%.1f\n",
                                precip, percent_snow, rho_snow, t_precip);
		}
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
