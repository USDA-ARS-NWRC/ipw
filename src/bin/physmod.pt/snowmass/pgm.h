#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "" program
 */

#define DE      10.0	/* MJ/mb m^3; bulk transfer coef. for stable */
			/* conditions; see Male and Gray Handbook of snow */
#define DH      5.74	/* MJ/mb m^3; bulk transfer coef. for H */

#define CANALB  0.3	/* canopy albedo */
 
#define MINALB  0.50    /* minimum possible albedo; Aguado, 1985 */
#define MAXALB  0.88    /* maximum snow albedo */
#define ALBINC  0.004   /* albedo decay increment (Coughlan Diss.) */
#define HEATINC 30.0    /* heat increment parameter (Coughlan Diss). */
 

extern double	melt(double snow_E_deficit, double tavg, double tmin,
		     double tdew, double precip, double solrad, double ithermf,
		     double lai, double albedo, double snowpack, double u,
		     double elev, double delta_t);

extern double	pm(double ta, double ea, double u, double Rn, double lai,
		   double cond);

extern double	rainint(double ppt, double tavg, double tdew, double solrad,
		       double ithermf, double u, double intercept_coef,
		       double lai, double delta_t);

extern double	snowint(double ppt, double solrad, double ithermf, double tavg,
		       double ppt_intercept_coef, double lai, double albedo,
		       double tmin, double u, double tdew, double delta_t);

#endif
