#define T0	300.0

#include	<unistd.h>
#include	"ipw.h"
#include        "pgm.h"
#include 	"envphys.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_i = {
		'i', "invert MSE equation to calculate vapor pressure",
	};

	static OPTION_T *optv[] = {
		&opt_i,
		0
	};

	int	inv;
	int	n;

	double	ea;
	double	elev;
	double	mixrat;
	double	mse;
	double	press;
	double	ta;
	double	temp;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/***	PROCESS ARGS	***/

	inv = got_opt(opt_i);

	/*	get input data	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO)) {
		if (inv)
			printf("Input ta, mse, and elev:\n");
		else
			printf("Input ta, ea, and elev:\n");
	}

	n = 0;

	while (scanf("%lf %lf %lf", &ta, &temp, &elev) != EOF) {

		/*	set temp to K	*/
		ta += FREEZE;

		/*	calculate total (vpres +dry air) air pressure	*/

		press = HYSTAT(SEA_LEVEL, T0, STD_LAPSE,
			       (elev/1000.), GRAVITY, MOL_AIR);

		/*	if "inv" set, calculate vapor pressure	*/

		if (inv) {
			mse = temp;
			mixrat = INV_MSE(elev, ta, mse);
			printf("%.2f\n", (mixrat * press) / (MOL_H2O / MOL_AIR));
		}
		/*	else, calculate moist static energy	*/

		else {
			ea = temp;
			mixrat = ( (MOL_H2O / MOL_AIR) * ea ) / press;
			printf("%.2f\n", MSE(elev, ta, mixrat));
		}

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
