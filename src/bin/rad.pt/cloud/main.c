#include	<unistd.h>

#include	"ipw.h"
#include        "envphys.h"
#include	"pgm.h"

#define C 2.4 /* C coef (Bristow & Campbell, 1984) */

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};

        int     n;
	double	B;
        double  tmax;
        double  tmin1;
        double  tmin2;
        double  deltaT;
        double  deltaT_ave;
	double	cloudiness;

   /* get args */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /*
    * if input is from a tty, then output prompt to standard error.
    * (standard error is used in case standard output is re-directed.)
    */
	if (isatty(STDIN_FILENO))
        	fprintf(stderr,
			"Input Tmax, Tmin1, Tmin2, deltaT_ave;\n");

   /* read input data and do calculations */
 
        n = 0;
        while (scanf("%lf %lf %lf %lf",
		     &tmax, &tmin1, &tmin2, &deltaT_ave) == 4) {
 
                n++;

		if (tmax < tmin1 || tmax < (tmin1 + tmin2) / 2)
			error("tmax < tmin or tmax < (tmin1 + tmin2) / 2, \
tmax=%g, tmin1=%g, tmin2=%g", tmax, tmin1, tmin2);
 
		deltaT = tmax - ((tmin1 + tmin2) / 2.0);

		B = 0.036 * exp ( -0.154 * deltaT_ave);

		cloudiness = exp (-1.0 * B * pow(deltaT, C));
 
                /*      output results to stdout        */
 
                printf("%3.2f\n", cloudiness);
        }
 
        if (n <= 0)
                error("bad or empty infile");
        

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
