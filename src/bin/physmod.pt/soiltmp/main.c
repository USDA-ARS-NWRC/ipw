#include	<math.h>
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

	double	Tmax;
	double	Tmin;
	double	Tave;
	double	lag;
	double	ta_lag;
	double	A;
	double	A_damp;
	double	Tgavg;
	double	Ta;
	double	Tg;
	double  P;	
        double  Amp;
	double  ta_func;
	double  tg_func;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/* check stdin for re-direct 	*/

	if (isatty(STDIN_FILENO)) {
		fprintf(stderr,
			"Input Tmax, Tmin, Tg_avg, P, Ta_lag, Tg_lag, Amp;\n");
	}


/*	read input data and do calculations	*/

	n = 0;

	if (scanf("%lf %lf %lf %lf %lf %lf %lf",
	      &Tmax, &Tmin, &Tgavg, &P, &ta_lag, &lag, &Amp) == 7) {

		if ((Tmin > Tmax)) {
			error("Min. temp (%lf) > Max. temp (%lf)", Tmin,
			      Tmax);
		}

		else {
			A = 0.50*(Tmax - Tmin);
			A_damp = Amp * (0.50*(Tmax - Tmin));
			Tave = (Tmin + Tmax) / 2.0;
		}

	/* convert to radians and calculate function for each time in Period */
		while (n < ((int) P)) {
			ta_func = sin(((2*M_PI)*(((float)n+1)-(ta_lag-(P/4))))/P);
			tg_func = sin(((2*M_PI)*(((float)n+1)-(lag-(P/4))))/P);
			Ta = Tave + (A * ta_func);
			Tg = Tgavg + (A_damp * tg_func);	
			printf("%d\t%.6f\t%.2f\t%.6f\t%.2f\n", n+1,
				ta_func, Ta, tg_func, Tg);
			n++;
		}
	}
	else {
		error("bad or empty input file");
	}

	ipwexit(EXIT_SUCCESS);
}	
