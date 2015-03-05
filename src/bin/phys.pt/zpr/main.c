#include <math.h>
#include <unistd.h>

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_i = {
		'i', "invert pressure & temperature to compute elevation",
	};

	static OPTION_T *optv[] = {
		&opt_i,
		0
	};

	int	inv;
	int	n;

	double	elev;
	double	pa;
	double	ta;
	double	rho_air;


	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	inv = got_opt(opt_i);

	/*	get input elev or pa	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO)) {
		if (!inv)
			fprintf(stderr, "Input elev:\n");
		else
			fprintf(stderr, "Input pa and ta:\n");
	}

	n = 0;

		/*	calculate air pressure	*/

		if (inv) {
			while (scanf("%lf %lf", &pa, &ta) != EOF) {
				ta += FREEZE;
				elev = INV_HYSTAT(SEA_LEVEL, STD_AIRTMP, 0.0,
					pa, ta, GRAVITY, MOL_AIR);
				rho_air = GAS_DEN(pa, MOL_AIR, ta);

				printf("%.3f\t%.1f\n", rho_air, elev*1000.0);
				n++;
			}
		}
		/*	else, calculate potential temperature	*/

		else {
			while (scanf("%lf", &elev) != EOF) {
				pa = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
					(elev/1000.), GRAVITY, MOL_AIR);
				printf("%.1f\n", pa);
				n++;
			}
		}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
