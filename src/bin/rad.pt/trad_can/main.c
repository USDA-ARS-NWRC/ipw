#include <math.h>

#include <unistd.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "radiation.h"

#define	EMISS_V	0.99	/* vegetation canopy emissivity */
#define	EMISS_S	0.99	/* surface emissivity */

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_K = {
		'K', "input vegetation canopy and surface temperatures in K (default: C)",
	};

	static OPTION_T opt_v = {
		'v', "input vegetation canopy emissivity (default: 0.99)",
		REAL_OPTARGS, "emiss_v",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "input surface emissivity (default: 0.99)",
		REAL_OPTARGS, "emiss_s",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_K,
		&opt_v,
		&opt_s,
		0
	};

	int	K;
	int	n;

	double	emiss_v;	/* emissivity of canopy */
	double	emiss_s;	/* emissivity of surface */
	double	exint3;		/* result of third-order exponential integral */
	double	lai;		/* leaf area index */
	double	lw_in;		/* input lw from atmosphere */
	double	lw_out_s;	/* lw exitance from surface */
	double	lw_up_v;	/* upward lw from canopy */
	double	lw_up_tot_a_a;	/* reflected atmos lw from thru canopy to sky*/
	double	lw_up_tot_b_a;	/* reflected atmos lw from surface to canopy */
	double	lw_up_tot_a_s;	/* upward lw from surface thru canopy to sky */
	double	lw_up_tot_b_s;	/* upward lw from surface to canopy */
	double	lw_up_tot_a_v;	/* reflected canopy lw from surface thru canopy to sky */
	double	lw_up_tot_b_v;	/* reflected canopy lw from surface back to canopy */
	double	lw_dn_v;	/* emitted canopy lw to surface */
	double	lw_dn_tot_a;	/* total downward lw from atmosphere to surface */
	double	lw_dn_tot_s;	/* total surface lw reflected back to surface from canopy */
	double	lw_dn_tot_v;	/* total downward lw from canopy to surface */
	double	lw_in_total;	/* total downward lw to surface */
	double	lw_out_total;	/* total upward lw from surface */
	double	lw_out_total_a;	/* total upward lw to atmos (above canopy) */
	double	lw_net;		/* net surface lw */
	double	lw_ref_coef;	/* lw reflectance coef */
	double	lw_trans_coef;	/* lw transmissivity coef */
	double	tau;		/* lai / 2.0 */
	double	Tv;		/* canopy temperature */
	double	Ts;		/* surface temperature */

	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	K  = got_opt(opt_K);

	/*	check for user-prescribed canopy and surface emissivities	*/
	/*	or set defaults	*/

	emiss_v = (n_args(opt_v) == 0) ? EMISS_V : real_arg(opt_v, 0);
	emiss_s = (n_args(opt_s) == 0) ? EMISS_S : real_arg(opt_s, 0);

	/*	get input lw_in, lai, Tv, Ts 	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "Input lw_in, lai, Tv, Ts:\n");

	n = 0;

	while (scanf("%lf %lf %lf %lf", &lw_in, &lai, &Tv, &Ts) != EOF) {

		/*	set Tv and Ts to K if necessary	*/
		if (!K) {
			Tv += FREEZE;
			Ts += FREEZE;
		}

		/*	check for reasonable lai	*/
		if ((lai < 0.0) || (lai > 10))
			error("lai (%f) out of range: 0 to 10", lai);

		/*	set canopy transmissivity and scattering coefficients	*/

		tau = lai / 2.0;	/* set optical depth */

		exint3 = exp_int3(tau);	/* call exponetial int routine */

		lw_ref_coef = 0.5 * (1.0 - emiss_v) * (1.0 - 2.0 * exint3);
		lw_trans_coef = fabs (2.0 * exint3 + lw_ref_coef);

		/*	calculate below canopy thermal radiation	*/
		
		/*	up and down lw radiation from canopy	*/
		lw_up_v = (1.0 - (2.0 * exint3)) *
			    (emiss_v * STEF_BOLTZ * Tv*Tv*Tv*Tv);
		lw_dn_v = lw_up_v;

		/*	total downward lw from canopy	*/
		lw_dn_tot_v = lw_dn_v / (1.0 - lw_ref_coef * (1.0 - emiss_v));

		/*	total canopy lw reflected by surf back to canopy	*/
		lw_up_tot_b_v = (1.0 - emiss_v) * lw_dn_tot_v;

		/*	total canopy lw reflected through canopy to sky	*/
		lw_up_tot_a_v = lw_up_v + lw_trans_coef * lw_up_tot_b_v;

		/*	lw exitance from the surface	*/
		lw_out_s = emiss_s * STEF_BOLTZ * Ts*Ts*Ts*Ts;

		/*	total lw from surface reflectd back down from canopy	*/
		lw_dn_tot_s = lw_ref_coef * lw_out_s /
				(1.0 - lw_ref_coef * (1.0 - emiss_v));

		/*	total surface lw incident on canopy	*/
		lw_up_tot_b_s = lw_out_s + (1.0 - emiss_v) * lw_dn_tot_s;

		/*	total surface lw through canopy to sky	*/
		lw_up_tot_a_s = lw_trans_coef * lw_up_tot_b_s;

		/*	lw from the atmosphere	*/
		lw_dn_tot_a = (lw_trans_coef * lw_in) /
				(1.0 - lw_ref_coef * (1.0 - emiss_v));

		/*	total lw from sky reflected by surface to canopy	*/
		lw_up_tot_b_a = (1.0 - emiss_v) * lw_dn_tot_a;

		/*	total lw from sky reflected through canopy to sky	*/
		lw_up_tot_a_a = (lw_ref_coef * lw_in) + (lw_trans_coef * lw_up_tot_b_a);

		/*	total lw input to surface	*/
		lw_in_total = lw_dn_tot_a + lw_dn_tot_v + lw_dn_tot_s;

		/*	total lw output from surface	*/
		lw_out_total = lw_up_tot_b_a + lw_up_tot_b_v + lw_up_tot_b_s;

		/*	net surface lw	*/
		lw_net = lw_in_total - lw_out_total;

		/*	total upward lw to the sky (above canopy)	*/
		lw_out_total_a = lw_up_tot_a_a + lw_up_tot_a_v + lw_up_tot_a_s;
		
		printf("%.2f\t%.2f\t%.2f\t%.2f\n",
			lw_in_total, lw_out_total, lw_net, lw_out_total_a);

		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
