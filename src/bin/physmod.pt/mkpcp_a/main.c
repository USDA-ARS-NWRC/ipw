#include	<unistd.h>

#include	"ipw.h"
#include	"pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_a = {
		'a', "temperature is air temperature (dew point is default)",
	};
	
	static OPTION_T *optv[] = {
		&opt_a,
		0
	};

	int	crt;
	int	n;
	int	a;
	int 	airtemp;

	double  precip;
	double  temp;
	double	percent_snow;
	double	rho_snow;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);
	
/*	check a flag		*/
	airtemp = got_opt(opt_a);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		if (airtemp)
		   fprintf(stderr,
		     "Input precip (kg (mm H2O)), t_air (C);\n");
		else 
		   fprintf(stderr,
		     "Input precip (kg (mm H2O)), t_precip (C);\n");
	}

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf",
                     &precip, &temp) == 2) {

		n++; 

		calc_snow(airtemp, temp, &percent_snow, &rho_snow);

		if (crt) {
		   if (airtemp) 
			printf("Precip_Mass = %.1f Percent_Snow = %.2f Snow_Density = %.3f Air_Temp = %.1f\n",
                                precip, percent_snow, rho_snow, temp);
                   else
                   	printf("Precip_Mass = %.1f Percent_Snow = %.2f Snow_Density = %.3f Precip_Temp = %.1f\n",
                                precip, percent_snow, rho_snow, temp);         
		}
		else {
			printf("%.1f\t%.2f\t%.3f\t%.1f\n",
                                precip, percent_snow, rho_snow, temp);
		}
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
