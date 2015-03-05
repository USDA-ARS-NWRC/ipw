#include	<unistd.h>

#include	"ipw.h"
#include        "envphys.h"
#include	"pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_m = {
		'm', "input vapor pressure in mb (Pa is default)",
	};

	static OPTION_T opt_K = {
		'K', "output dewpoint temp. in K (C is default)",
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&opt_K,
		0
	};

        int     n;
        int     mb;
        int     K;
        double  vpress;
        double  d_point;


   /* get args */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	mb = got_opt(opt_m);
	K  = got_opt(opt_K);

   /*
    * if input is from a tty, then output prompt to standard error.
    * (standard error is used in case standard output is re-directed.)
    */
	if (isatty(STDIN_FILENO))
        	fprintf(stderr, "Input vapor pressure;\n");

   /* read input data and do calculations */
 
        n = 0;
        while (scanf("%lf", &vpress) == 1) {
 
                n++;
 
 
                if(mb)
                        vpress = vpress * 100.0;
 
                /*      calculate dew point temp        */
 
                d_point = dew_point(vpress);
 
                /*      convert dew point temp to C if in K, & !K       */
 
                if(!K)
                        d_point -= FREEZE;
 
                /*      output results to stdout        */
 
                printf("%6.2f\n", d_point);
        }
 
        if (n <= 0)
                error("bad or empty infile");
        

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
