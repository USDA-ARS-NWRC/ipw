#define PP 1.26				/* Priestley-Taylor Parameter */

#include	<math.h>
#include 	"ipw.h"
#include 	"envphys.h"
#include 	"pgm.h"

int
main (
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};
 
	int	crt;
	int	n;

	double	p_yr; 		/* annual precipitation (mm) */
	double  p_day;		/* daily precipitation (mm) */
	double  daylen;		/* daylength (s) */
	double	pet;		/* estimated daily potential ET (kg/m2 day) */
	double	Rn;		/* average daily net radiation (w/m2) */
	double	arad;		/* average daily available radiation (w/m2) */
	double	slope;		/* slope of vapor pressure-temperature curve (mb/K) */
	double  delta;	
	double  latv;		/* latent heat of H2O vaporization (J/kg) */
	double  tx;		/* maximum daily air temperature (C) */
	double  tn;		/* minimum daily air temperature (C) */
	double  ta;		/* average daily air temperature (C) */
	double t0;
	double t1;
	double t2;
	double tnk;		/* minimum daily air temp. (K) */
	double txk;		/* maximum daily air temp. (K) */
	double e0;
	double e1;
	double e2;
	double eaest;		/* estimated daily vapor pressure (Pa) */
	double tkest;		/* estimated dewpoint (K) */
	double tdest;		/* estimated dewpoint (C) */
	double eatn;		/* minimum temperature based vapor pressure (Pa) */
	double ef;		/* fraction of pet over p_yr(Pa) */
	double efcub;		/* cubic relat. between ef and dewpoint, tn fraction */ 
	double gamma;		/* psychrometric constant (mb/K) */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	check stdin for re-direct	*/

	if (isatty(fileno(stdin))) {
		crt = TRUE;
		fprintf(stderr,
		   "input Rn, tx, tn, daylen, pcp, PcpYR;\n");
	}
	else
		crt = FALSE;


/*	read input data and do calculations	*/

	n = 0;


	while(scanf("%lf %lf %lf %lf %lf %lf",
	      &Rn, &tx, &tn, &daylen, &p_day, &p_yr) == 6) {

			n++; 

/* Calculate available energy (Campbell 1977, Running and Coughlan 1988) */

	arad = Rn-(0.1*Rn);

/* Calculate daylight average air temperature (Running and Coughlan 1988) */

	ta = (((tx+tn)/2.0)+tx)/2.0;

/* Calculate weighting factor (change in ea with temperature) */

/* convert to kelvin */

	t0 = (ta+FREEZE);
	t1 = ((ta+0.5)+FREEZE);
	t2 = ((ta-0.5)+FREEZE);
	tnk = (tn+FREEZE);
	txk = (tx+FREEZE);

/* calculate saturation vapor pressures in Pa */

	e0 = sati(t0);
	e1 = sati(t1);
	e2 = sati(t2);
	eatn = sati(tnk);

/* calulate delta in mb; gamma = psychrometric const. */
/* gamma calculated using Running bgc method  */

	gamma = 0.646+0.0006*ta;
	slope = (e1-e2)/100.0;
	delta = slope/(slope+gamma);

/* calculate latent heat of vaporization (J/kg, Running bgc) */

	latv = (2.501-0.0024*ta)*1.0e+6;

/* Calculate pet (kg/m2 day) */

	pet = ((PP*delta*arad)*daylen)/latv;

/* Calculate adjusted humidity (Kimball et al. 1997) */

	ef = pet/p_yr; /* assume water density=1000.0 kg/m3 for pet */

	efcub = 1.0028-1.4444*ef+12.3125*pow(ef,2.0)-32.766*pow(ef,3.0);

	tkest = tnk*(-0.126976+0.000564*(txk-tnk)+1.121442*(efcub)); 

/* convert K to C */

	tdest = tkest-FREEZE;

/* calculate adjusted humidity */

	eaest = sati(tkest);

	if(p_day > 0.0) {
   	   tdest = tn;
   	   eaest = eatn;
	}

        if (crt) {
     	   printf("EADJ=%.2f ETMIN=%.2f;\n", eaest, eatn);
        }
        else { 
           printf("%.2f\n", eaest);
	}
        }
 
        if (n <= 0)
                error("bad or empty input file");
 
        ipwexit(EXIT_SUCCESS);
}
