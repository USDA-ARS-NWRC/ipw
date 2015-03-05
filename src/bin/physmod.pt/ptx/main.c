#include	<math.h>
#include	<unistd.h>

#include	"ipw.h"
#include        "envphys.h"
#include        "pgm.h"

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

        double  PP;
        double  le;
        double  Rn;
        double  arad;
        double  slope;
        double  delta;
        double  ta;
        double tak;
        double t1;
        double t2;
        double sat_vpres;
        double sat_vpres1;
        double sat_vpres2;
        double Gamma;
        double l10;


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	crt = isatty(STDIN_FILENO);
	if (crt) {
		fprintf(stderr, "Input Rn (W/m2), ta (C), PP (approx. 1.26);\n");
	}

/*	read input data and do calculations	*/

	n = 0;


	while(scanf("%lf %lf %lf", &Rn, &ta, &PP) == 3) {

			n++; 

/* Calculate available energy (Campbell 1977, Running 1992) */
 
        arad = Rn - (0.1 * Rn);
 
/* Calculate weighting factor (change in qs with temperature) */

	/* convert to kelvin */
                        tak = (ta + FREEZE);
                        t1 = ((ta + 0.5)+273.15);
                        t2 = ((ta - 0.5)+273.15);

        /* calculate saturation vapor pressures in Pa */
 
      if (tak <= 273.15) {
                sat_vpres = 100*pow(1.e1,-9.09718*((273.15/tak)-1.) 
                   - 3.56654*log(273.15/tak)/log(1.e1) + 8.76793e-1
                   *(1.-(tak/273.15)) + log(6.1071)/log(1.e1));
 
                sat_vpres1 = 100*pow(1.e1,-9.09718*((273.15/t1)-1.)
                   - 3.56654*log(273.15/t1)/log(1.e1) + 8.76793e-1
                   *(1.-(t1/273.15)) + log(6.1071)/log(1.e1));
 
                sat_vpres2 = 100*pow(1.e1,-9.09718*((273.15/t2)-1.)
                   - 3.56654*log(273.15/t2)/log(1.e1) + 8.76793e-1
                   *(1.-(t2/273.15)) + log(6.1071)/log(1.e1));
 
        }
        else {
 
            l10 = log(1.e1);
 
            sat_vpres = -7.90298*(BOIL/tak-1.) + 5.02808*log(BOIL/tak)/l10 -
            1.3816e-7*(pow(1.e1,1.1344e1*(1.-tak/BOIL))-1.) +
            8.1328e-3*(pow(1.e1,-3.49149*(BOIL/tak-1.))-1.) +
            log(SEA_LEVEL)/l10;
 
            sat_vpres1 = -7.90298*(BOIL/t1-1.) + 5.02808*log(BOIL/t1)/l10 -
            1.3816e-7*(pow(1.e1,1.1344e1*(1.-t1/BOIL))-1.) +
            8.1328e-3*(pow(1.e1,-3.49149*(BOIL/t1-1.))-1.) +
            log(SEA_LEVEL)/l10;
 
            sat_vpres2 = -7.90298*(BOIL/t2-1.) + 5.02808*log(BOIL/t2)/l10 -
            1.3816e-7*(pow(1.e1,1.1344e1*(1.-t2/BOIL))-1.) +
            8.1328e-3*(pow(1.e1,-3.49149*(BOIL/t2-1.))-1.) +
            log(SEA_LEVEL)/l10;
 
 
 
        sat_vpres = pow(1.e1,sat_vpres);
        sat_vpres1 = pow(1.e1,sat_vpres1);
        sat_vpres2 = pow(1.e1,sat_vpres2);
 
        }
                        Gamma = 0.646 + 0.0006 * ta;
                        slope = (sat_vpres1 - sat_vpres2)/100;
                        delta = slope / (slope + Gamma);
 
 
/* Calculate le (W/m2) */
 
                le = -1.0 * (PP * delta * arad);
 
                if (crt) {
			printf("LE= %.2f\n", le);
		}
		else {
			printf("%.2f\n", le);
		}
        }
 
        if (n <= 0)
                error("bad or empty input file");
 
        ipwexit(EXIT_SUCCESS);
}
