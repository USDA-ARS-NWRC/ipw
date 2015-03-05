#include	<unistd.h>
#include	<math.h>

#include	"ipw.h"
#include        "envphys.h"
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

	double	PP;	/* Priestley Taylor coefficient */
	double	le;	/* latent energy flux Wm^-2 */
	double	Rn;	/* net all-wave solar radiation Wm^-2 */
	double	G;	/* conductive energy flux Wm^-2 */
	double	slope;	/* slope of VP-temperature curve mb^K-1 */
	double  delta;
	double  ta;	/* air temperature C */
	double ts;	/* surface temperature C */
	double twa;
	double t0;      /* air temperature K */
	double t1;
	double t2;
	double t3;	/* surface temperature K */
	double ets;	/* saturation vapor pressure at surface Pa */
	double ea;	/* vapor pressure of air Pa */
	double e0;	/* saturation vapor pressure of air Pa */
	double e1;
	double e2;
	double Gamma;	/* psychrometric constant */
	double ws;
	double w;
	double p;	/* wet bulb temperature Pa */
	double L;	/* latent heat of vaporization Jkg^-1 */
	double diff;	/* available energy Wm^-2 */


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		fprintf(stderr,
		   "Input Rn, G, ta, ts, ea, PP;\n");
	}

/*	read input data and do calculations	*/

	n = 0;


	while(scanf("%lf %lf %lf %lf %lf %lf",
	      &Rn, &G, &ta, &ts, &ea, &PP) == 6) {

			n++; 

/* Calculate weighting factor (change in saturation vapor pressure with temperature) */

	/* convert to kelvin */
			t0 = (ta + FREEZE);
			t1 = ((ta + 0.5)+FREEZE);
			t2 = ((ta - 0.5)+FREEZE);
			t3 = (ts + FREEZE);

	/* calculate saturation vapor pressures in Pa */

			ets = sati(t3);
			e0 = sati(t0);
			e1 = sati(t1);
			e2 = sati(t2);

	/* calulate delta in mb; Gamma = psychrometric const. */
	/* (Running and Coughlan 1988)  */

			Gamma = 0.646 + 0.0006 * ta;
			slope = (e1 - e2)/100.0;
			delta = slope / (slope + Gamma);

/* Calculate Priestly Taylor parameter, PP */

	/* calculate latent heat of vaporization, Jkg-1,  (Running and Coughlan 1988) */

		L = (2.501 - 0.0024 * ta) * 1000000.0;

	/* calculate wet bulb temperature (Byers 1974, p.122, 110) */

		p = (1.292 - 0.000428 * ta)*100000.0; 

		ws = 0.622 * (e0 / (p - e0));

		w = 0.622 * (ea / (p - ea));

       	twa = ((ta+FREEZE) - (((L / CP_AIR) * fabs(ws - w)))) - FREEZE;

        if (twa > ta) {
		twa = ta;
	}
		
	/* calculate PP (Pereira and Nova 1992, p. 3) */

        if (PP <= 0.0) {
	  PP = 1.0+((Gamma/slope)*fabs((1.0-(fabs(ts-ta)/fabs(ts-twa))))); 

	   if (PP <= 1.0) {
		    PP = 1.0;
	   }
	   if (PP > (1.0/delta)) {
		   PP = (1.0/delta);
	   }
        }

/* Calculate le (W/m2) */
		diff = -1.0*(Rn - G);

		le = (PP * delta * (diff));

                        if (crt) {
     				printf("LE=%.2f; PP=%.2f;\n", le, PP);
                        }
                        else
                		printf("%.2f\n", le);
        }
 
        if (n <= 0)
                error("bad or empty input file");
 
        ipwexit(EXIT_SUCCESS);
}
 

