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

	double	pm();
	double  le;	/* latent energy flux Wm^-2 */
	double  ts;	/* surface temperature, C */
	double  ta;	/* air temperature, C */
	double	ea;	/* vapor pressure of air, Pa */
	double	u;	/* wind speed, ms^-1 */
	double	Rn;	/* net all-wave radiation Wm^-2*/
	double	G;	/* surface conductive energy flux, Wm^-2 */
	double	z0;	/* surface roughness length, m */
	double	z;	/* measurement height, m */
	double rc;	/* canopy resistance, sm^-1 */
	double ra;	/* aerodynamic resistance, sm^-1 */
	double ratio;	/* ratio of vpdflx to radflx */
	double radflx;	/* portion of LE due to radiation term in PM, Wm^-2 */
	double vpdflx;	/* portion of LE due to aerodynamic term in PM, Wm^-2 */
	double Rib;	/* bulk Richardson number */
	double F;	/* unitless stability function */


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		fprintf(stderr,
		   "Input ts, ta, ea, u, Rn, G, z0, z, rc;\n");
	}

/*	read input data and do calculations	*/

	n = 0;

	while(scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf",
	      &ts, &ta, &ea, &u, &Rn, &G, &z0, &z, &rc) == 9) {

	   n++; 

	   le = pm(ts, ta, ea, u, Rn, G, z0, z, rc, &ratio, &radflx, &vpdflx, &ra, &Rib, &F);

		if (crt) {
	printf("LE=%.2f; RATIO=%.2f; RAD=%.2f; VPD=%.2f; Ra=%.2f; Rib=%.2f; F=%.2f;\n",
				le, ratio, radflx, vpdflx, ra, Rib, F);
		}
		else
printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", le, ratio, radflx, vpdflx, ra);
	}

	if (n <= 0)
		error("bad or empty input file");

	ipwexit(EXIT_SUCCESS);
}
