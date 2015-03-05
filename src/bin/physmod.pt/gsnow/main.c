#include	<unistd.h>

#include 	"ipw.h"
#include	"snow.h"
#include	"pgm.h"

#define		MAXTS	274.0	/* maximum tsno correctible */

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "measurement site elevation (m)",
		REAL_OPTARGS, "elev",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_K = {
		'K', "input temperatures in K (default: C)",
	};

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_K,
		0
	};

	int	crt;
	int	K;
	int	n;

	double	elev;
	double	g_wet;
	double	g_dry;
	double	g_moist;
	double	k_g_wet;
	double	k_g_dry;
	double	k_g_moist;
	double	k_s;
	double	kcs;
	double	pa;
	double	rho;
	double	tg;
	double	tsno;
	double	zg;
	double	zs;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	process optional args	*/

        if (got_opt(opt_z)) {
                elev = real_arg(opt_z, 0);
        }
        else {
                elev = 0.0;
        }

/*	check K flag	*/

	K  = got_opt(opt_K);

/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		crt = TRUE;
		fprintf(stderr, "Input rho tsno tg zs zg;\n");
	}
	else
		crt = FALSE;

/*	set pa from site elev	*/

	pa = HYSTAT (SEA_LEVEL, STD_AIRTMP, STD_LAPSE, (elev / 1000.0),
			GRAVITY, MOL_AIR);

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf %lf %lf %lf",
		&rho, &tsno, &tg, &zs, &zg) == 5) {

		n++; 

		if (!K) {
			tsno  += FREEZE;
			tg += FREEZE;
		}
		if ((tsno < 0.0) || (tg < 0.0))
			error("bad temperature input");

		/*	check tsno	*/
		if (tsno > FREEZE) {
			/* if < threshold, set to 273.16 */
			if (tsno < MAXTS) {
				warn("tsno = %8.2f; set to 273.16\n", tsno);
				tsno = FREEZE;
			}
			else
				error("bad tsno = %.2f", tsno);
		}

		/*	check for positive layer thicknesses	*/
		if (zs < 0.0)
			error("zs must be positive");
		if (zg < 0.0)
			error("zg must be positive");

		/*	set effective soil conductivity	*/
		k_g_wet = efcon (KT_WETSAND, tg, pa);
		k_g_dry = efcon (KT_DRYSAND, tg, pa);
		k_g_moist = efcon (KT_MOISTSAND, tg, pa);

		/*	calculate G	*/
		/*	set snow conductivity	*/
		kcs = KTS(rho);
		k_s = efcon (kcs, tsno, pa);

		g_wet = ssxfr (k_s, k_g_wet, tsno, tg, zs, zg);
		g_dry = ssxfr (k_s, k_g_dry, tsno, tg, zs, zg);
		g_moist = ssxfr (k_s, k_g_moist, tsno, tg, zs, zg);

		if (crt)
			printf("G(wet) = %.2f; G(dry) = %.2f; G(moist) - %.2f\n",
				g_wet, g_dry, g_moist);
		else
			printf("%.2f %.2f %.2f\n", g_wet, g_dry, g_moist);
	}
	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
