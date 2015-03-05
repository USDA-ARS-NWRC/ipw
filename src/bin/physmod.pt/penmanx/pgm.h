#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "pmx" program
 */

extern double	pm(double ts, double ta, double ea, double u, double Rn,
		   double G, double z0, double z, double rc, double *ratio,
		   double *radflx, double *vpdflx, double *ra, double *Rib,
		   double *F);

#endif
