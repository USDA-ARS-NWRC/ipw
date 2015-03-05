#include	<unistd.h>

#include	"ipw.h"
#include	"pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};

	int	crt;
	int	n;

	double  le;
	double  ta;
	double	ea;
	double	u;
	double	Rn;
	double  lai;
	double	cond;
	double	ra;
	double	radflx;
	double	vpdflx;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		fprintf(stderr,
		   "Input ta (C), ea (Pa), u (m/s), Rn (W/m2), lai, cond (m/s);\n");
	}

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf %lf %lf %lf %lf",
                     &ta, &ea, &u, &Rn, &lai, &cond) == 6) {
 

		n++; 

		le = pm(ta, ea, u, Rn, lai, cond, &radflx, &vpdflx, &ra);
 

		if (crt) {
			printf("LE= %.2f RADFLX= %.2f VPDFLX= %.2f Ra= %.2f\n",
                                le, radflx, vpdflx, ra);
		}
		else {
			printf("%.2f\t%.2f\t%.2f\t%.2f\n", le, radflx, vpdflx, ra);
		}
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
