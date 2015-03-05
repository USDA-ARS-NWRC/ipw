#include "ipw.h"
#include "solar.h"
#include "_solar.h"

datetime_t *
sunrise(
	double          lat,	/* latitude (radians)  */
	double          lon,	/* longitude (radians) */
	int             year,	/* year (e.g., 1997)   */
	int             month,	/* month (1 - 12)      */
	int             day)	/* day (1 - 31)        */
{
	return (_sunpos(lat, lon, year, month, day, SUNRISE));
}
