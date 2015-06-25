#include <math.h>
#include "ipw.h"
#include "pgm.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_u = {
			'u', "cosine of incidence angle",
			REAL_OPTARGS, "cos",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_0 = {
			'0', "ignore cosine <= 0.0",
	};
	static OPTION_T opt_o = {
			'w', "single-scattering albedo",
			REAL_OPTARGS, "omega",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_t = {
			't', "optical depth (0 -> infinite)",
			REAL_OPTARGS, "tau",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_g = {
			'g', "asymmetry factor",
			REAL_OPTARGS, "g",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_r = {
			'r', "reflectance of substrate (if <= 0, set to 0)",
			REAL_OPTARGS, "R0",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_s = {
			's', "direct beam irradiance (if <= 0, set to mu_0 * S0 = 1)",
			REAL_OPTARGS, "S0",
			OPTIONAL, 1, 1
	};
	static OPTION_T opt_m = {
			'd', "delta-Eddington method (Meador-Weaver hybrid is default)"
	};
	static OPTION_T *optv[] = {
			&opt_u,
			&opt_0,
			&opt_t,
			&opt_o,
			&opt_g,
			&opt_r,
			&opt_s,
			&opt_m,
			0
	};
	double          g;		/* asymmetry parameter		 */
	double          ga;		/* asymmetry parameter		 */
	double          gam[4];		/* scattering function params	 */
	double          rho0;		/* substrate reflectance	 */
	double          s0;		/* solar illumination		 */
	double          tau0;		/* optical depth		 */
	double          t0;		/* optical depth		 */
	double          u0;		/* cos (illum angle)		 */
	double          omega;		/* single-scat albedo		 */
	double          wa;		/* single-scat albedo		 */
	double          refl;		/* reflectance (output)		 */
	double          trans;		/* transmittance (output)	 */
	double          btrans;		/* direct transmittance (out)	 */
	bool_t          got_omega;	/* ? omega from command line	 */
	bool_t          got_asymm;	/* ? g from command line	 */
	bool_t          got_tau0;	/* ? tau0 from command line	 */
	bool_t          got_mu0;	/* ? u0 from command line	 */
	bool_t          got_r0;		/* ? rho0 from command line	 */
	bool_t          got_s0;		/* ? s0 from command line	 */
	bool_t          mw;		/* ? Meador-Weaver hybrid	 */
	bool_t          cz0;		/* ? ignore mu0 <= 0.0		 */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 * which arguments on command line?
	 */
	got_omega = got_opt(opt_o);
	got_asymm = got_opt(opt_g);
	got_tau0 = got_opt(opt_t);
	got_mu0 = got_opt(opt_u);
	got_r0 = got_opt(opt_r);
	got_s0 = got_opt(opt_s);
	mw = !got_opt(opt_m);
	cz0 = got_opt(opt_0);
	/*
	 * get arguments from command line
	 */
	if (got_mu0) {
		u0 = real_arg(opt_u, 0);
		if (!cz0) {
			if ( (u0 <= 0) || (u0 > 1) ) {
				error("u0 must be > 0 and <= 1");
			}
		}
	}
	if (got_tau0) {
		tau0 = real_arg(opt_t, 0);
		if (tau0 <= 0)
			tau0 = HUGE_VAL;
	}
	if (got_omega)
		omega = real_arg(opt_o, 0);
	if (got_asymm)
		g = real_arg(opt_g, 0);
	if (got_r0) {
		rho0 = real_arg(opt_r, 0);
		if (rho0 < 0)
			rho0 = 0;
	}
	if (got_s0) {
		s0 = real_arg(opt_s, 0);
		if (s0 <= 0)
			s0 = (got_mu0 && u0 != 0) ? 1 / u0 : 1;
	}

	/*
	 * delta-Eddington scaling
	 */
	if (got_omega && got_asymm && got_tau0) {
		wa = omega;
		ga = g;
		t0 = tau0;
		if (!mw)
			delta_edd(&wa, &ga, &t0);
	}

	/*
	 * gamma's for phase function for all input; 0 = delta-Edd option; 1 =
	 * M-W hybrid
	 */

	if (got_omega && got_mu0 && got_asymm && got_tau0)
		if (!cz0) {
			mwgamma(u0, wa, ga, gam, mw);
		} else {
			if (u0 > 0.0) {
				mwgamma(u0, wa, ga, gam, mw);
			}
		}

	/*
	 * just one problem if all input from command line
	 */
	if (got_omega && got_asymm && got_tau0 && got_mu0 && got_r0 && got_s0) {

		/*
		 * solution to twostream
		 */
		if (cz0) {
			/* ignore mu0 <= 0.0 */
			if (u0 <= 0.0) {
				refl = trans = btrans = 0.0;
			} else {
				if (twostream(gam, wa, u0, t0, rho0,
						&refl, &trans, &btrans) == ERROR) {
					error("twostream error");
				}
			}
		} else {
			/* error on mu0 <= 0.0 */

			if (twostream(gam, wa, u0, t0, rho0, &refl, &trans, &btrans)
					== ERROR) {
				error("twostream error");
			}
		}
		/*
		 * output
		 */
		if (u0<= 0.0) {
			printf("reflectance %g\n", 0.0);
			printf("transmittance %g\n", 0.0);
			printf("direct transmittance %g\n", 0.0);
			printf("upwelling irradiance %g\n", 0.0);
			printf("total irradiance at bottom %g\n", 0.0);
			printf("direct irradiance normal to beam %g\n", 0.0);
		} else {
			printf("reflectance %g\n", refl);
			printf("transmittance %g\n", trans);
			printf("direct transmittance %g\n", btrans);
			printf("upwelling irradiance %g\n", refl * u0 * s0);
			printf("total irradiance at bottom %g\n", trans * u0 * s0);
			printf("direct irradiance normal to beam %g\n", btrans * s0);
		}
	}

	/*
	 * otherwise some input from stdin
	 */
	else {
		/*
		 * output order
		 */
		if (isatty(fileno(stdout))) {
			printf("mu_0, tau_0, omega_0, g, rho_0, S_0");
			printf(", refl, trans, btrans");
			printf(", up flux top, down flux bot, dir bot\n");
		}

		for (;;) {
			/*
			 * read values from stdin
			 */
			if (!got_mu0) {
				if (scanf("%lf", &u0) == EOF)
					break;
			}
			if (!got_tau0) {
				if (scanf("%lf", &tau0) == EOF)
					break;
			}
			if (!got_omega) {
				if (scanf("%lf", &omega) == EOF)
					break;
			}
			if (!got_asymm) {
				if (scanf("%lf", &g) == EOF)
					break;
			}
			if (!got_r0) {
				if (scanf("%lf", &rho0) == EOF)
					break;
			}
			if (!got_s0) {
				if (scanf("%lf", &s0) == EOF)
					break;
			}
			/*
			 * echo input
			 */
			printf("%g %g %g %g %g %g",
					u0, tau0, omega, g, rho0, s0);
			/*
			 * delta-Eddington scaling
			 */
			if (!got_omega || !got_asymm || !got_tau0) {
				t0 = tau0;
				wa = omega;
				ga = g;
				if (!mw)
					delta_edd(&wa, &ga, &t0);
			}
			/*
			 * gamma's for phase function
			 */
			if (!got_omega || !got_mu0 || !got_asymm || !got_tau0)
				if (!cz0) {
					mwgamma(u0, wa, ga, gam, mw ? 1 : 0);
				} else {
					if (u0 > 0.0) {
						mwgamma(u0, wa, ga, gam, mw ? 1 : 0);
					}
				}
			/*
			 * solution to twostream
			 */
			if (cz0) {
				/* ignore mu0 <= 0.0 */
				if (u0 <= 0.0) {
					refl = trans = btrans = 0.0;
				} else {
					if (twostream(gam, wa, u0, t0, rho0,
							&refl, &trans, &btrans) == ERROR) {
						error("twostream error");
					}
				}
			} else {
				/* error on mu0 <= 0.0 */
				if (twostream(gam, wa, u0, t0, rho0,
						&refl, &trans, &btrans) == ERROR) {
					error("twostream error");
				}
			}
			/*
			 * print output
			 */
			if (u0<= 0.0) {
				printf(" %g %g %g %g %g %g\n",
						refl, trans, btrans,
						0.0,
						0.0,
						0.0);
			} else {
				printf(" %g %g %g %g %g %g\n",
						refl, trans, btrans,
						refl * u0 * s0,
						trans * u0 * s0,
						btrans * s0);
			}
		}
	}

	exit(0);
}
