#include <math.h>

#include "ipw.h"
#include "radiation.h"

#define SEMI	1

int
twostream(
		double  *gamma,   /* vector of gamma coefficients    */
		double   omega,   /* single-scattering albedo        */
		double   mu0,     /* cosine of incidence angle       */
		double   tau,     /* optical depth of layer          */
		double   r0,      /* reflectance of substrate        */

		/* output variables */

		double  *refl,    /* reflectance of layer            */
		double  *trans,   /* total transmittance of layer
				       (compensated for mu0) */
		double  *btrans)  /* direct transmittance of layer
				       (compensated for mu0) */
{
	double          alph1;
	double          alph2;
	double          denrt;
	double          em;
	double          ep;
	double          et;
	double          gam1;
	double          gam2;
	double          gam3;
	double          gam4;
	double          gmx;
	double          gpx;
	double          omx;
	double          opx;
	double          rm;
	double          rp;
	double          xi;

	/*
	 * intermediate variables
	 */
	gam1 = gamma[0];
	gam2 = gamma[1];
	gam3 = gamma[2];
	gam4 = gamma[3];
	alph1 = gam1 * gam4 + gam2 * gam3;
	alph2 = gam2 * gam4 + gam1 * gam3;
	if (gam1 < gam2) {
		usrerr("gam1 (%g) < gam2 (%g)", gam1, gam2);
		return (ERROR);
	}
	/*
	 * (Hack - gam1 = gam2 with conservative scattering.  Need to fix
	 * this.
	 */
	if (gam1 == gam2) {
		gam1 += DBL_EPSILON;
	}
	xi = sqrt((gam1 - gam2) * (gam2 + gam1));
	em = exp(-tau * xi);
	et = exp(-tau / mu0);
	ep = exp(tau * xi);
	gpx = xi + gam1;
	opx = mu0 * xi + 1;

	/*
	 * semi-infinite?
	 */
	if ((em == 0 && et == 0) || (ep == HUGE_VAL)) {
		*refl = omega * (gam3 * xi + alph2) / (gpx * opx);
		*btrans = *trans = 0;
		return (SEMI);
	}
	else {

		/*
		 * more intermediate variables, needed only for finite case
		 */
		omx = 1 - mu0 * xi;
		gmx = gam1 - xi;
		rm = gam2 - gmx * r0;
		rp = gam2 - gpx * r0;

		/*
		 * denominator for reflectance and transmittance
		 */
		denrt = ep * gpx * rm - em * gmx * rp;

		/*
		 * reflectance
		 */
		*refl = (omega * (ep * rm * (gam3 * xi + alph2) / opx
				- em * rp * (alph2 - gam3 * xi) / omx)
				+ 2 * et * gam2
				* (r0 - ((alph1 * r0 - alph2) * mu0 + gam4
						* r0 + gam3) * omega / (omx * opx)) * xi) / denrt;

		/*
		 * transmittance
		 */
		*trans = (et * (ep * gpx * (gam2 - omega * (alph2
				- gam3 * xi) / omx)
				- em * gmx * (gam2 - omega * (gam3 * xi + alph2) / opx))
				+ 2 * gam2 * (alph1 * mu0 + gam4) * omega * xi /
				(omx * opx)) / denrt;

		/*
		 * direct transmittance
		 */
		*btrans = et;

		assert(*refl >= 0);
		assert(*trans >= 0);
		assert(*btrans >= 0);
		assert(*trans >= *btrans * mu0);

		return (OK);
	}
}
