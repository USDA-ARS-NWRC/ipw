#include	<math.h>

#include	"ipw.h"
#include	"radiation.h"

double
net_therm(
	double	ta,		/* air temperature (K)			   */
	double	ts,		/* surface temperature (K)		   */
	double	pa,		/* air pressure (Pa)			   */
	double	ea,		/* vapor pressure (Pa)			   */
	double	surfemiss,	/* surface emissivity (0.0 - 1.0)	   */
	double	skyfac,		/* terrain skyfactor (0.0 - 1.0)	   */
	double	lapse,		/* temp. lapse rate (deg/m) (negative up!) */
	double	z)		/* elevation (m)			   */

{
	double	airemiss;
	double	in;
	double	landfac;
	double	landin;
	double	net;
	double	skyin;
	double  ts_to_the_4th;

	landfac = 1.0 - skyfac;
	
	airemiss = brutsaert(ta, lapse, ea, z, pa);

	ts_to_the_4th = pow(ts, 4.0);
	skyin = (airemiss * STEF_BOLTZ * pow(ta, 4.0)) * skyfac;
	landin = (surfemiss * STEF_BOLTZ * ts_to_the_4th) * landfac;

	in = skyin + landin;

	net = in - (surfemiss * STEF_BOLTZ * ts_to_the_4th);

	return(net);
}
