#include "ipw.h"
#include "pgm.h"

#define NSUBST		4
#define TEMP_DEFAULT	(-5.)

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_r = {
		'r', "real part only (default both)"
	};

	static OPTION_T opt_i = {
		'i', "imaginary part only (default both)"
	};

	static OPTION_T opt_d = {
		'd', "dielectric function instead of refractive index"
	};

	static OPTION_T opt_s = {
		's', "{ ice, water }",
		STR_OPTARGS, "substance",
		REQUIRED, 1,
	};

	static OPTION_T opt_t = {
		't', "temperature, deg C (default -5)",
		REAL_OPTARGS, "temp",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
		'u', "wavelength units: (-u help for choices)",
		STR_OPTARGS, "units",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&opt_i,
		&opt_s,
		&opt_t,
		&opt_u,
		&opt_d,
		0
	};

	COMPLEX_T      *((*func[NSUBST]) ());
	double         (*xform)(double);

	COMPLEX_T      *m;		/* -> refractive index	 */
	COMPLEX_T       _m;		/* (storage for m)	 */
	bool_t          dielectric;	/* ? dielectric fn	 */
	bool_t          imagpart;	/* ? print imag part	 */
	bool_t          realpart;	/* ? print real part	 */
	char           *subst;		/* -> substance name	 */
	char           *units;		/* units of wave meas	 */
	char            lambda[64];	/* wave storage vector	 */
	double          k;		/* imag part		 */
	double          n;		/* real part		 */
	double          temp;		/* temperature (C)	 */
	double          z;		/* scaling factor	 */
	int             i;		/* index		 */
	int             n_Subst;	/* # substances		 */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	m = &_m;

 /* Parse Options */

 /*** units for input wavelength data ***/
	units = (n_args(opt_u) <= 0) ? NULL : str_arg(opt_u, 0);
	wavechoice(units, &xform);

 /*** real and/or imaginary parts ***/
	realpart = got_opt(opt_r);
	imagpart = got_opt(opt_i);
	if (!realpart && !imagpart) {
		realpart = imagpart = 1;
	}

 /*** dielectric function instead of refractive index? ***/
	dielectric = got_opt(opt_d);

 /*** which substances? ***/
	n_Subst = n_args(opt_s);
	if (n_Subst > NSUBST) {
		error("maximum is %d substances", NSUBST);
	}
	for (i = 0; i < n_Subst; i++) {
		subst = str_arg(opt_s, i);
		if (streq(subst, "ice")) {
			func[i] = refice;
		}
		else if (streq(subst, "water")) {
			func[i] = refh2o;
		}

 /* add other substances here as we incorporate them */

		else {
			error("don't know refractive index for substance %s",
			      subst);
		}
	}

 /*** temperature ***/
	temp = (n_args(opt_t) == 0) ? TEMP_DEFAULT : real_arg(opt_t, 0);

 /*** main loop ***/
	while (scanf("%s", lambda) != EOF) {
		z = (xform != NULL) ? (*xform) (atof(lambda)) : atof(lambda);
		printf("%s", lambda);
		for (i = 0; i < n_Subst; i++) {
			m = (*func[i]) (m, z, temp);
			n = m->re;
			k = m->im;
			if (dielectric) {
				if (realpart)
					printf("\t%.4f", (n - k) * (n + k));
				if (imagpart)
					printf("\t%.3e", 2 * n * k);
			}
			else {
				if (realpart)
					printf("\t%.4f", n);
				if (imagpart)
					printf("\t%.3e", k);
			}
		}
		putchar('\n');
	}

	exit(0);
}
