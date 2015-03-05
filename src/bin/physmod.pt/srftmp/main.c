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
	double	t;
	double	A;
	double	Tavg;
	double	result;
	double  P;	
        double  lag;
	double  function;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/* check stdin for re-direct 	*/

	if (isatty(STDIN_FILENO)) {
		fprintf(stderr, "Input Tmax, Tmin, Tavg, P, lag, t;\n");
	}


/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf %lf %lf %lf %lf",
	      &Tmax, &Tmin, &Tavg, &P, &lag, &t) == 6) {

		n++;

		if ((Tmin > Tmax)) {
			error("Min. temp (%lf) > Max. temp (%lf)", Tmin,
			      Tmax);
		}

		else {
			A = 0.50*(Tmax - Tmin);

/* convert to radians and calculate function */

			function = sin(((2.0 * M_PI/P)*(t+lag)));
			result = Tavg + (A * function);	
			printf("%.2f\n", result);
		}
	}
	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}	
