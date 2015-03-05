/*
** NAME
**	_bdinit -- calculates parameters needed to evaluate sensible
**		   and latent heat exchange
**
** SYNOPSIS
**
**	#include "_envphys.h"
**
**	void
**	_bdinit(
**	     double z,       |* height of measurements			    *|
**	     double z0,      |*    "   "       "  (may be roughness length) *|
**	     double t,       |* temperature at z			    *|
**	     double t0,      |*      "      "  z0			    *|
**	     double q,       |* specific humidity at z 			    *|
**	     double q0,      |*    "        "     "  z0			    *|
**	     double u,       |* wind speed at z 			    *|
**	     double u0,      |*  "     "   "  z0 (may be zero) 		    *|
**
**	   |* output variables *|
**
**	     double *ustar,  |* friction velocity			    *|
**	     double *tstar,  |* scaling temperature			    *|
**	     double *qstar)  |* scaling humidity			    *|
**
** HISTORY
**	Jun 1995	Converted from QDIPS to IPW by J. Domingo, OSU
**			(formerly called bdfunc.c)
*/

#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"
#include	"_envphys.h"

void
_bdinit(
	double z,	/* height of measurements			    */
	double z0,	/*    "   "       "       (may be roughness length) */
	double t,	/* temperature at z				    */
	double t0,	/*      "      "  z0				    */
	double q,	/* specific humidity at z 			    */
	double q0,	/*    "        "     "  z0 			    */
	double u,	/* wind speed at z 				    */
	double u0,	/*  "     "   "  z0 (may be zero) 		    */

   /* output variables */

	double *ustar,	/* friction velocity				    */
	double *tstar,	/* scaling temperature				    */
	double *qstar)	/* scaling humidity				    */
{
	double	alpha, beta, lzz0, k, Ri, Ricrit, zeta, psi1, psi2, x, y;

	alpha = 16;
	beta = 4.7;
	Ricrit = 1 / beta;
	lzz0 = log(z/z0);
	k = VON_KARMAN;

	/* convert t to potential temp, w.r.t. surface */
	t += DALR * z;

	/* bulk Richardson no at geom mean ht */
	Ri = ri_no(z,z0,t,t0,u,u0);

	/* laminar flow if Ri >= Ricrit */
	if (Ri >= Ricrit) {
		*ustar = *tstar = *qstar = 0.0;
		return;
	}

	if (Ri == 0)
		*tstar = psi1 = psi2 = 0;
	else {
		/* dimensionless ht zeta=z/L, L = Obukhov length */
		if (Ri < 0.) {
			zeta = Ri;

			/* x = 1/phi-sub-m */
			y = sqrt(1.-alpha*zeta);
			x = sqrt(y);

			/* the Businger-Dyer psi-functions */
			psi1 = 2 * log((1+x)/2) + log((1+y)/2) -
			    2 * atan(x) + M_PI_2;
			psi2 = 2 * log((1+y)/2);
		}

		else {
			zeta = Ri / (1.-beta*Ri);
			psi1 = psi2 = -beta * zeta;
		}

		*tstar = k * (t-t0) / (lzz0 - psi2);
	}

	*ustar = k * (u-u0) / (lzz0 - psi1);
	*qstar = k * (q-q0) / (lzz0 - psi2);

	return;
}
