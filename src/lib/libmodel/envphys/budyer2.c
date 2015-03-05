#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"
#include	"_envphys.h"

int
budyer2(
	double	zu,	/* height of wind speed measurement (m)		 */
	double	zt,	/* height of temp & vapor press measurements (m) */
	double	z0,	/* roughness length (m)				 */
	double	t,	/* temperature at zt (K)			 */
	double	t0,	/*      "      "  z0 (K)			 */
	double	e,	/* vapor pressure at zt (Pa)			 */
	double	e0,	/*   "      "     "  z0 (Pa)			 */
	double	u,	/* wind speed at zu (m/s)			 */
	double	p,	/* pressure (Pa)				 */

    /* output variables */

	double	*h,	/* sensible heat exchange (W/m^2)		 */
	double	*le)	/* latent heat exchange (W/m^2)		   	 */
{
	double	cp, ustar, tstar, qstar, xlh, q, q0, aden;
	double	tgm, egm;
	int	rc;

	if (zu <= z0)
		error("budyer2: zu<=z0");
	if (zt <= z0)
		error("budyer2: zt<=z0");

	rc = OK;
	if (e > satw(t)) {
		rc = ERROR;
		e = satw(t);
		warn("budyer2: e reset to saturation value");
	}
	if (e0 > sati(t0)) {
		rc = ERROR;
		e0 = sati(t0);
		warn("budyer2: e0 reset to saturation value");
	}

	/* geometric mean t, e */
	tgm = sqrt(t*t0);
	egm = sqrt(e*e0);

	cp = CP_AIR;

	/* temperature-dependent latent heat of vaporization */
	xlh = LH_VAP(tgm);

	q = SPEC_HUM(e,p);
	q0 = SPEC_HUM(e0,p);

	/* air density based on virtual temperature at mean e */
	aden = GAS_DEN(p,MOL_AIR, VIR_TEMP(tgm,egm,p));

	/*
	 *  Previously, this function called the library function "bdf2":
	 *
	 *	\* friction velocity ustar, scaling temp tstar, scaling *\
	 *	\* specific humidity qstar *\
	 *	if (bdf2(zu,zt,z0,t,t0,q,q0,u, &ustar, &tstar, &qstar))
	 *		error("bdf2 failed in budit2");
	 *
	 *  But the "bdf2" function would simply called the library function
	 *  "bdfunc" with the same parameters that had been passed to bdf2.
	 *  Therefore, for simplicity (i.e., the avoid the need to port bdf2
	 *  from QDIPS to IPW), we just call bdfunc directly.  See the file
	 *  "bdf2.old" for details.
	 *  J. Domingo, June 1995
	 *
	 *  "bdfunc" now called "_bdinit", Sept 1997
	 */
	_bdinit(zu, z0, t, t0, q, q0, u, 0., &ustar, &tstar, &qstar);

	*h = cp * aden * ustar * tstar;
	*le = aden * xlh * qstar * ustar;

	return(rc);
}
