#include	<unistd.h>

#include	"ipw.h"
#include 	"pgm.h"
#include        "envphys.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_K = {
		'K', "input dewpoint temp. in K (default: C)",
	};

	static OPTION_T opt_m = {
		'm', "output vapor pressure in mb (default: Pa)",
	};

	static OPTION_T *optv[] = {
		&opt_K,
		&opt_m,
		0
	};

	int	n;
	int	mb;
	int	K;
	double	sat_vpres;
	double	d_point;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* see if more args */

	K  = got_opt(opt_K);
	mb = got_opt(opt_m);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO))
		fprintf(stderr,"Input dew point temp.;\n");

	/*	read input data and do calculations  	*/

	n = 0;
	while (scanf("%lf", &d_point) == 1) {

		n++;

		/*	convert dew point temp to K if in C	*/
		if(!K)
			d_point += FREEZE;

		if (d_point < 0.0)
			error("unrealistic dew point temperature: %g", d_point);
	
		/*	calculate saturation vapor pressure	*/
		sat_vpres = sati(d_point);

		/*	output results to stdout	*/
		if(mb)
			printf("%6.2f\n", sat_vpres/100.0);
		else
			printf("%6.2f\n", sat_vpres);
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
