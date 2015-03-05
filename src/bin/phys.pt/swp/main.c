#include 	<math.h>
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

	/*	from Table 2, Saxton et al., pg 1033,
		Soil Society of America Journal, Vol. 50, 1986	*/

	double	a = -4.396;
	double	b = -0.0715;
	double	c = -4.880e-4;
	double	d = -4.285e-5;
	double	e = -3.140;
	double	f = -2.22e-3;
	double	g = -3.484e-5;
	double	h = 0.332;
	double	j = -7.251e-4;
	double	k = 0.1276;
	double	m = -0.108;
	double	n = 0.341;
	double	p = 12.012;
	double	q = -7.55e-2;
	double	r = -3.8950;
	double	t = 3.671e-2;
	double	u = -0.1103;
	double	v = 8.7546e-4;

	double	ms_cmh = 3.6e5;

	double	aa;
	double	bb;

	double	psi;		 /* water potential, kPa */
	double	psi_e = 0.0;	 /* water potential (kPa) at air entry */
	double	h2o;		 /* soil water content (m^3/m^3) */
	double	h2o_sat = 0.0;	 /* s_h2o at saturation (m^3/m^3) */
	double	h2o_10 = 0.0;	 /* s_h2o at 10kPa (m^3/m^3) */
	double	hycon;		 /* hydraulic conductivity (m/s) */
	double	hycon_sat = 0.0; /* saturated hydraulic cond (m/s) */
	double	sand;		 /* percent sand (eg 40) */
	double	clay;		 /* percent clay (eg 30) */

	int	i;

	/* check args */
	
	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/*	check stdin for re-direct	*/

	if (isatty(STDIN_FILENO))
		fprintf(stderr,
		"Input sand, clay (0-100), soil H2O content (by volume);\n");

	/*	read input data and do calculations  	*/

	i = 0;

	while (scanf("%lf %lf %lf", &sand, &clay, &h2o) == 3) {

		i++;

		/*	set constants (aa, bb) */

		aa = exp( a + (b * clay) + (c * sand * sand) +
			(d * sand * sand * clay) ) * 100.0;

		bb = e + (f * clay * clay) + (g * sand * sand) +
			(g * sand * sand * clay);

		/*	set saturated soil water content (h2o_sat) */
		
		h2o_sat = h + (j * sand) + (k * log10(clay));

		/*	set air entry water potential (psi_e) */

		psi_e = 100.0 * ( m + (n * h2o_sat));

		/*	set 10kPa soil water content (h2o_10) */

		h2o_10 = exp( (2.302 - log(aa)) / bb);

		/*	set hydraulic conductivity */

		hycon = 2.778e-6 *
			exp( p + (q * sand) +
			((r + (t * sand)+(u * clay)+(v * clay * clay)) /
			h2o ));

		hycon_sat = 2.778e-6 *
			exp( p + (q * sand) +
			((r + (t * sand)+(u * clay)+(v * clay * clay)) /
			h2o_sat ));

		/**	set soil water potential (psi) **/

		psi = aa * pow(h2o, bb);

		if (psi <= 10.0) {
			psi = 10.0 - ( (h2o - h2o_10) *
			      (10.0 - psi_e) /
			      (h2o_sat - h2o_10) );
		}

		/*	output results to stdout	*/

			printf("%6.0f %% %8.1f %.2e %.2e\n",
				h2o*100.0, psi, hycon, ms_cmh*hycon);
	}

	if (i > 0) {
		printf("\n\n%.2e %.2e\n%8.1f\n%5.1f %5.1f\n",
			hycon_sat, ms_cmh*hycon_sat, psi_e,
			100.0*h2o_10, 100.0*h2o_sat);
	}
	else {
		error("bad or empty infile");
	}
	
	ipwexit(EXIT_SUCCESS);
}
