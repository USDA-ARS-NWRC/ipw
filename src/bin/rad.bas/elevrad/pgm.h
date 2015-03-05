#ifndef PGM_H
#define PGM_H
 
#include "IPWmacros.h"
#include "envphys.h"

 /*
 * look-up tables passed externally between radtbl and elevrad
 */

extern fpixel_t       *beam;		/* -> beam radiation	 */
extern fpixel_t       *diffuse;		/* -> diffuse radiation	 */

extern void	elevrad(int fdi, int fdo);
extern void	init_tau(int fdi, int fdo, double elev, double mu0,
			 double tau, double omega, double gfact, double R0,
			 double S0, int *nbits);
extern void	radtbls(int fdi, double elev, double mu0, double tau,
			double omega, double gfact, double R0, double S0,
			fpixel_t *bval, fpixel_t *dval);

#endif

