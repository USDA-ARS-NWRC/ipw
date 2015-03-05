#include	<stdio.h>
#include	<unistd.h>

#include	"ipw.h"
#include	"pgm.h"
#include	"envphys.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_r = {
		'r', "precipitation is rain (snow is default)",
	};

	static OPTION_T opt_K = {
		'K', "input temperatures are in K (C is default)",
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&opt_K,
		0
	};

	bool_t	crt;
	int	K;
	int	n;
	int	rain;

	double	adv;
	double	pcp;
	double	pd;
	double	prho;
	double	pspm;
	double	pt;
	double	ts;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	process optional args	*/

/*	check rain flag	*/
	rain = got_opt(opt_r);

/*	check K flag	*/

	K  = got_opt(opt_K);

/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		crt = TRUE;
		fprintf(stderr, "Input prho pd pt ts\n");
	}
	else
		crt = FALSE;

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf %lf %lf", &prho, &pd, &pt, &ts) == 4) {

		n++; 

		if (!K) {
			pt = C_TO_K(pt);
			ts = C_TO_K(ts);
		}
		if ((pt < 0.0) || (ts < 0.0))
			error("bad temperature input");

		if (!rain) {
			if (pt >= FREEZE)
				pcp = (CP_WATER(pt)+CP_ICE(FREEZE))/2.0;
			else
				pcp = CP_ICE(pt);
		}
		else
			pcp = CP_WATER(pt); 

		pspm = pd * prho;

		adv = heat_stor(pcp,pspm,(pt-ts));

		if (crt)
			/* printf("Adv = %.2f\n", adv); */
			printf("Adv = %.2f\t%.2f\n", adv, pcp);
		else
			/* printf("%.2f\n", adv); */
			 printf("Adv = %.2f\t%.2f\n", adv, pcp);
	}
	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
