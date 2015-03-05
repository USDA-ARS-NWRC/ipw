#include	<unistd.h>

#include	"ipw.h"
#include        "pgm.h"
#include	"envphys.h"

#define		AH_MIN	0.0
#define		VP_MIN	0.0

int
main(
	int	argc,
	char	**argv)
{

	static OPTION_T opt_K = {
		'K', "input ta in K (default: C)",
	};

	static OPTION_T opt_i = {
		'i', "inverse: vp from abs_hum"
	};

	static OPTION_T *optv[] = {
		&opt_K,
		&opt_i,
		0
	};

	int	n;		/* input counter	*/
	int	K;		/* input ta in K	*/
	int	inverse;	/* use inverse func	*/

	double	abs_hum;
	double	ta;
	double	vpress;

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* see if more args */

	K = got_opt(opt_K);
	inverse = got_opt(opt_i);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO)) {
		if (inverse)
			fprintf(stderr,"Input vapor pressure and air temperature;\n");
		else
			fprintf(stderr,"Input absolute humidity and air temperature;\n");
	}

	/*	read input data and do calculations  	*/

	n = 0;
	if (inverse) {
		/* calculate absolute humidity from vapor pressure */

		while (scanf("%lf %lf", &vpress, &ta) == 2) {

			/*	check ta, and convert if necessary	*/
			if (!K)
				ta += FREEZE;

			/*	check input vpress	*/
			if (vpress < VP_MIN)
				error("input VP (%lf) < 0.0", vpress);

			n++;
			
			/* calculate absolute humidity (in gm/m^3)	*/

			abs_hum = VP2AH(vpress, ta) * 1000.0;

			/*	output results to stdout	*/

			printf("%6.4f\n", abs_hum);
		}
	} else	{
		/* calculate vp from abs_hum*/

		while (scanf("%lf %lf", &abs_hum, &ta) == 2) {

			/*	check ta, and convert if necessary	*/
			if (!K)
				ta += FREEZE;

			/*	check input abs_hum	*/
			if (abs_hum < AH_MIN)
				error("input abs_hum (%lf) < 0.0", abs_hum);

			n++;

			/* convert abs_hum to kg/m^3 */

			abs_hum /= 1000.0;

			/*	calculate vp	*/

			vpress = AH2VP(abs_hum, ta); 

			/* output vp to stdout */

			printf("%6.2f\n", vpress);
		}
	}

	if (n <= 0)
		error("bad or empty infile");
	
	ipwexit(EXIT_SUCCESS);
}
