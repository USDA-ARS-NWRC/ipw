#include	<unistd.h>
#include	"ipw.h"
#include        "pgm.h"
#include        "envphys.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_i = {
		'i', "invert sea-level or potential temperature"
	};

	static OPTION_T opt_l = {
		'l', "lapse rate (def: -6.5 deg C/km)",
		REAL_OPTARGS, "val",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_s = {
		's', "sea-level temp. (def: 26.84 C (300K))",
		REAL_OPTARGS, "val",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_L = {
		'L', "do simple linear adjustment based on lapse rate & elev"
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_l,
		&opt_s,
		&opt_L,
		0
	};

	int	inv;		/* Invert temperature flag	*/
	int	linear;		/* linear adjustment flag	*/
	double	T0;		/* assumed sea-level temp.	*/
	double	lapse;		/* lapse rate 			*/

	int	n;

	double	elev;
	double	press;
	double	ta;
	double	temp;
	double	theta;
	double	sl_ta;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/***	PROCESS ARGS	***/

	/* check inversion and linear options */

	inv    = got_opt (opt_i);
	linear = got_opt (opt_L);

	/* access numerical options */

	if (got_opt (opt_l)) {
	    lapse = real_arg (opt_l, 0);
	} else {
	    lapse = STD_LAPSE;
	}

	if (got_opt (opt_s)) {
	    T0 = (real_arg (opt_s, 0)) + FREEZE;
	} else {
	    T0 = 300.0;
	}

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO)) {
		if (inv && linear)
			fprintf(stderr, "Input sea-level ta and elev:\n");
		else if (inv)
			fprintf(stderr, "Input theta and elev:\n");
		else
			fprintf(stderr, "Input ta and elev:\n");
	}

	n = 0;

	while (scanf("%lf %lf", &temp, &elev) != EOF) {

		/*	set temp to K	*/
		temp += FREEZE;

		/*	check for linear	*/
		if (linear) {
			/*	if "inv" calculate air temperature */
			if (inv) {
				sl_ta = temp;
				ta = sl_ta + lapse * (elev/1000.0);
				temp = ta;
			}

			/*	else, calculate sea-level temperature	*/
			else {
				ta = temp;
				sl_ta = ta - lapse * (elev/1000.0);
				temp = sl_ta;
			}
		}
		/*	if not linear	*/
		else {
			/*	calculate air pressure	*/

			press = HYSTAT(SEA_LEVEL, T0, lapse,
				(elev/1000.), GRAVITY, MOL_AIR);

			/*	if "inv" calculate air temperature */
			if (inv) {
				theta = temp;
				ta = INV_POT_TEMP(theta, press);
				temp = ta;
			}

			/*	else, calculate potential temperature	*/
			else {
				ta = temp;
				theta = POT_TEMP(ta, press);
				temp = theta;
			}
		}
		/*	set temp back to C	*/

		temp -= FREEZE;

		printf("%.2f\n", temp);

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
