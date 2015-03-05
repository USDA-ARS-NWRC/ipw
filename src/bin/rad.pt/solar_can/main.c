#include <math.h>

#include <unistd.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "radiation.h"

#define	SS_ALB_V	0.25

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_v = {
		'v', "input vegetation single scattering albedo (default: 0.25)",
		REAL_OPTARGS, "alb_v",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_v,
		0
	};

	int	n;

	double	alb_v;		/* canopy albedo */
	double	alb_s;		/* surface albedo */
	double	exint3;		/* result of third-order exponential integral */
	double	lai;		/* leaf area index */
	double	mu_0;		/* cosine of solar zenith angle */
	double	sw_in;		/* input solar from atmosphere */
	double	sw_in_dir;	/* input direct solar from atmosphere */
	double	sw_in_dif;	/* diffuse solar from atmosphere */
	double	sw_dn_v;	/* downward solar from canopy */
	double	sw_up_a;	/* upward solar from canopy */
	double	sw_up_tot_a;	/* total above canopy solar */
	double	sw_up_tot_v;	/* total upward below canopy solar */
	double	sw_dn_v_dir;	/* direct solar below the canopy */
	double	sw_dn_v_dif;	/* diffuse solar below the canopy */
	double	sw_dn_tot_v;	/* total below canopy downward solar */
	double	sw_net;		/* net solar at surface */
	double	sw_ref_coef;	/* solar reflectance coef */
	double	sw_trans_coef;	/* solar transmissivity coef */
	double	tau;		/* lai / 2.0 */

	/***	PROCESS ARGS	***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/*	check for user-prescribed canopy single scattering albedo	*/
	/*	or set default	*/

	alb_v = (n_args(opt_v) == 0) ? SS_ALB_V : real_arg(opt_v, 0);

	/*	get input sw_in, sw_in_dir, mu0, lai, alb_s 	*/

	/*	check for stdin re-direct	*/
	if (isatty(STDIN_FILENO))
		fprintf(stderr, "Input sw_in, sw_in_dir, mu0, lai, alb_s:\n");

	n = 0;

	while (scanf("%lf %lf %lf %lf %lf",
			&sw_in, &sw_in_dir, &mu_0, &lai, &alb_s) != EOF) {

		/*	check for reasonable lai	*/
		if ((lai < 0.0) || (lai > 10))
			error("lai (%f) out of range: 0 to 10", lai);

		/*	check for reasonable mu_0	*/
		if ((mu_0 < -1.0) || (mu_0 > 1.0))
			error("mu_0 (%f) out of range: -1.0 to 1.0", mu_0);

		/*	if sun is up	*/
		if (mu_0 > 0.0) {
			/* set canopy transmissivity and scattering coefficients */

			tau = lai / 2.0;	/* set optical depth */
		
			exint3 = exp_int3(tau);	/* call exponetial int routine */

			sw_ref_coef = (alb_v / 2.0) * (1.0 - (2.0 * exint3));
			sw_trans_coef = fabs (2.0 * exint3 + sw_ref_coef);

			/*	set solar diffuse	*/
			sw_in_dif = sw_in - sw_in_dir;

			/*	Sub-canopy downwelling direct and diffuse solar	*/
			sw_dn_v_dir = ( (alb_v / 2.0) * (1.0 - exp(-tau / mu_0)) *
					sw_in_dir + (sw_in_dir * exp(-tau / mu_0)) ) /
				      ( 1.0 - (sw_ref_coef * alb_s) );

			sw_dn_v_dif = ( sw_trans_coef * sw_in_dif ) /
				      ( 1.0 - (sw_ref_coef * alb_s) );

			/*	1. Downward below canopy solar radiation */
			sw_dn_v = (alb_v / 2.0) * (1.0 - exp(-tau / mu_0)) * sw_in_dir
					+ (sw_in_dir * exp(-tau / mu_0))
					+ (sw_trans_coef * sw_in_dif);

			/*	2. Upward above canopy solar radiation	*/
			sw_up_a = (alb_v / 2.0) * (1.0 - exp(-tau / mu_0)) * sw_in_dir
					+ sw_ref_coef * sw_in_dif;

			/*	3. Sub-canopy upwelling flux	*/
			/*	not needed	*/

			/*	4. Total sub-canopy downward solar radiation	*/
			sw_dn_tot_v = sw_dn_v / (1.0 - alb_s * sw_ref_coef);

			/*	5. Total exitance from the surface to canopy	*/
			sw_up_tot_v = alb_s * sw_dn_tot_v;

			/*	6. Total upwelling solar above canopy	*/
			sw_up_tot_a = sw_up_a + (sw_trans_coef * sw_up_tot_v);
	
			/*	net surface sw	*/
			sw_net = sw_dn_tot_v - sw_up_tot_v;

			printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
				sw_dn_v_dir, sw_dn_v_dif, sw_dn_tot_v,
				sw_up_tot_v, sw_net, sw_up_tot_a);

			n++;
		} else {
		/*	sun is down	*/
			sw_dn_v_dir = 0.0;
			sw_dn_v_dif = 0.0;
			sw_up_tot_a = 0.0;
			sw_dn_tot_v = 0.0;
			sw_up_tot_v = 0.0;
			sw_net = 0.0;

			printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
				sw_dn_v_dir, sw_dn_v_dif, sw_dn_tot_v,
				sw_up_tot_v, sw_net, sw_up_tot_a);

			n++;
		}
	}

	fprintf(stderr, "%d records processed\n", n);

	ipwexit(EXIT_SUCCESS);
}
