#include <unistd.h>
#include <math.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T *optv[] = {
		0
	};

	int	n;

	double	tmax;		/* max daily air temp. (C) */
	double	tmin;		/* min daily air temp. (C) */
	double  ppt_intercept_coef; /* m/lai */
	double	tdew;		/* daily dewpoint temp. (C) */
	double	ppt;		/* precipitation depth (m) */
	double	ithermf;	/* adjusted forest floor lw_in (J) */
	double	solrad;		/* adjusted sol_in (J) using daily tmax tmin */
	double  tavg;		/* mean daily air temperature (C) */
	double	lai;		/* one-sided leaf area index */
	double  runoff; 	/* runoff (m) */
	double  old_acc;	/* old snow on ground */
	double  snow;		/* snowfall (m) */
	double  albedo;		/* albedo */
	double  rain;		/* rainfall (m) */
	double  snowpack;	/* snow water equivilent (m) */
	double  heatsum;	/* snowpack heat sum (C) */
	double  albheatsum;	/* albedo heat sum (C) */
	double  totmelt;	/* snowmelt (m) */
	double  sublimation;	/* snow sublimation (m) */
	double  elev;		/* site elevation (m) */
	double  u;		/* wind speed (ms^-1) */
	double  delta_t;	/* time period of Solin (hrs) */


	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

/*	get input data and check stdin re-direct	*/

	if (isatty(STDIN_FILENO)) {
		fprintf(stderr, "INPUT: solar, thermal, Tmax, Tmin, dpt, pcp, lai, z, u, dt, intrcpt\n");
	}

	n = 0;

/* initialize model */

    albedo = MINALB;
    old_acc = 0.0001;
    albheatsum = 0.0;
    heatsum = 0.0;

	while (scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
			&solrad, &ithermf, &tmax, &tmin, &tdew, &ppt, &lai,
			&elev, &u, &delta_t, &ppt_intercept_coef) == 11) {

   if (ppt_intercept_coef < 0.0) {
      ppt_intercept_coef = 0.001;
   }

/* calculate average daytime temperature from tmax, tmin */
/* See (Hungerford et al. 1989 */

    tavg = (tmax+((tmax+tmin)/2.))/2.;

/* If wind speed = 0, compute parameterized wind speed (ms^-1) based on lai; */
/* this is just a guess for now */

    if (u <= 0.0) {
    u = MAX(0.5, (2.0-lai*0.375));
     }


/* check for precipitation, partition pcp as rain or snow based on tavg */
/* estimate pcp interception based on lai and evaporate from canopy */

    snow = snowint(ppt, solrad, ithermf, tavg, ppt_intercept_coef, lai, albedo, tmin, u, tdew, delta_t);

    rain = rainint(ppt, tavg, tdew, solrad, ithermf, u, ppt_intercept_coef, lai, delta_t);


/* update solar albedo if snowfall has occurred */
		
    if (snow > 0.0) {
        albheatsum = 0.0;
	albedo = MAXALB;
       }

    else {
        albheatsum = heatsum;

    albedo = MIN(albedo,(MAXALB-ALBINC*(albheatsum/HEATINC)));
    }


/* update snowpack depth */

    snowpack = MAX((old_acc+snow), 0.0);
    
/* update snowpack ripeness using degree day method */

    if (snowpack > 0.0) {

            heatsum = MAX((heatsum+tavg), (-1.0*HEATINC));
    }

    else {
        heatsum = 0.0;
    }


/* melt snow */

    totmelt = melt(heatsum, tavg, tmin, tdew, ppt, solrad, ithermf, lai, albedo, snowpack, u, elev, delta_t);

    snowpack = MAX((snowpack-totmelt),0.0);
    old_acc = snowpack; 


/* calculate runoff */

    if (heatsum > 0.0) {

        runoff = MAX((totmelt+rain), 0.0);

        sublimation = 0.0;
        }

    else {

        runoff = rain;

        sublimation = MAX(totmelt, 0.0);

        totmelt = 0.0;
        }

/* reset snowpack energy content to 0 if no snow on ground */

    if (old_acc <= 0.0) {

            heatsum = 0.0;
    }


    printf("M= %.6f R= %.6f S= %.6f SU= %.6f hs= %.6f rain= %.6f snow= %.6f albedo= %.6f\n",
	totmelt, runoff, snowpack, sublimation, heatsum, rain, snow, albedo);


		n++;
	}

	fprintf(stderr, "%d records processed\n", n);

    ipwexit(EXIT_SUCCESS);
}
