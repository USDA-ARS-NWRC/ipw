#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"
#include	"_envphys.h"

int
budyer(
	double	z,	/* upper height of wind speed & temp measurement (m) */
	double	z0,	/* upper height of wind speed & temp measurement (m),
			   (maybe roughness length)			     */
	double	t,	/* temperature at z (K)				     */
	double	t0,	/*      "      "  z0 (K)			     */
	double	e,	/* vapor pressure at z (Pa)			     */
	double	e0,	/*   "      "     "  z0 (Pa)			     */
	double	u,	/* wind speed at z (m/s)			     */
	double	u0,	/*  "     "   "  z0 (m/s)			     */
	double	p,	/* pressure (Pa)				     */

    /* output variables */

	double	*h,	/* sensible heat exchange (W/m^2)		     */
	double	*le)	/* latent heat exchange (W/m^2)		   	     */
{
	double	cp, xlh,  q, q0, aden, esat, ustar, tstar, qstar;
	int	rc;

	rc = OK;
	cp = CP_AIR;

	/* latent heat of vaporization at geometric mean temp */
	xlh = LH_VAP(sqrt(t*t0));

	/* check vapor pressures, reset if > saturation */
	esat = satw(t);
	if (e > esat) {
		rc = ERROR;
		e = esat;
		warn("budyer: e reset to saturation value");
	}
	esat = sati(t0);
	if (e0 > esat) {
		rc = ERROR;
		e0 = esat;
		warn("budyer: e0 reset to saturation value");
	}

	q = SPEC_HUM(e,p);
	q0 = SPEC_HUM(e0,p);

	/* air density based on virtual temperature at mean e */
	aden = GAS_DEN(p,MOL_AIR, VIR_TEMP(sqrt(t*t0),sqrt(e*e0),p));

	/* friction velocity ustar, scaling temp tstar, scaling */
	/* specific humidity qstar */
	_bdinit(z, z0, t, t0, q, q0, u, u0, &ustar, &tstar, &qstar);

	*h = aden * cp * ustar * tstar;
	*le = aden * xlh * ustar * qstar;

	return(rc);
}
