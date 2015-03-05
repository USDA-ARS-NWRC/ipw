/*
** NAME
**	_sunpos - compute sunrise or sunset time (GMT)
**
** SYNOPSIS
**	#include "_solar.h"
**
**	datetime_t *
**	_sunpos(
**		double          lat,	|* latitude (radians)  *|
**		double          lon,	|* longitude (radians) *|
**		int             year,	|* year (e.g., 1997)   *|
**		int             month,	|* month (1 - 12)      *|
**		int             day,	|* day (1 - 31)        *|
**		int             flag)	|* SUNRISE or SUNSET   *|
**
** DESCRIPTION
**	_sunpos returns a date-time structure which represents
**	either the sunrise or sunset time at a given latitude and longitude.
**	South latitudes and west longitudes are negative.
**	The date_time structure contains GMT time of local sunrise/sunset,
**	but it does not account for local horizons.
**
** RETURN VALUE
**	non-NULL
**		date-time (GMT) of sunrise/sunset
**
**	NULL
**		error detected, and mesage stored via usrerr routine
*/

#include <math.h>
#include <errno.h>

#include "ipw.h"
#include "_solar.h"

#define TOLERANCE	1.e-5

static double      xlat;
static double      xlon;
static datetime_t  dt_midnite;  /* midnite on given day */

static double      sunhorz(double tsec);

/* ------------------------------------------------------------------------ */

datetime_t *
_sunpos(
	double          lat,	/* latitude (radians)  */
	double          lon,	/* longitude (radians) */
	int             year,	/* year (e.g., 1997)   */
	int             month,	/* month (1 - 12)      */
	int             day,	/* day (1 - 31)        */
	int             flag)	/* SUNRISE or SUNSET   */
{
	double          a;		/* lower guess for root	 */
	double          b;		/* upper guess for root	 */
	double          cosom;		/* cosine omega		 */
	double          declin;		/* declination		 */
	double          dsec;		/* seconds-of-day	 */
	double          rv;		/* radius vec (not used) */
	double          sollon;		/* solar lon (not used)	 */
	datetime_t     *tp;		/* date-time answer      */

	assert(flag == SUNRISE || flag == SUNSET);

	xlat = lat;
	xlon = lon;

 /*
  * initialize global variable with midnight of given day
  */
	dt_midnite.year  = year;
	dt_midnite.month = month;
	dt_midnite.day   = day;
	dt_midnite.hour  = 0;
	dt_midnite.min   = 0;
	dt_midnite.sec   = 0;

 /*
  * value of declination at noon GMT on same day
  */
	tp = make_dt(year, month, day, 12, 0, 0);
	if (tp == NULL)
		return NULL;
	if (ephemeris(tp, &rv, &declin, &sollon) == ERROR) {
		return (NULL);
	}

 /*
  * approximate value of cos(omega) at rise/set
  */
	cosom = -tan(xlat) * tan(declin);

 /*
  * if no rise or set
  */
	if (fabs(cosom) > 1) {
		a = (flag == SUNRISE) ? 0 : SECS_IN_DAY;
		b = SECS_IN_DAY / 2;
	}
	else {
 /*
  * approximate half-daylength, seconds (i.e. 43200 omega / M_PI)
  */
		dsec = (SECS_IN_DAY / 2) * acos(cosom) / M_PI;
 /*
  * convert to local seconds-of-day, midnight = 0
  */
		if (flag == SUNRISE)
			dsec = SECS_IN_DAY / 2 - dsec;
		else
			dsec += SECS_IN_DAY / 2;
 /*
  * set high and low values for root search
  */
		a = (dsec > SECS_IN_HR / 2)
					    ? dsec - SECS_IN_HR / 2
					    : 0;
		b = (dsec < SECS_IN_DAY - SECS_IN_HR / 2)
						? dsec + SECS_IN_HR / 2
						: SECS_IN_DAY;
	}

 /*
  * convert to seconds GMT
  */
	if (xlon != 0.) {
		a -= 12 * SECS_IN_HR * xlon / M_PI;
		b -= 12 * SECS_IN_HR * xlon / M_PI;
	}
 /*
  * If root not spanned, we have either 24 h darkness or 24 h daylight.
  */
	if (sunhorz(a) * sunhorz(b) > 0) {
 /*
  * 24 hrs daylight: set answer to 00:00:00 if sunrise, 24:00:00 if
  * sunset
  */
		if (sunhorz(a) > 0) {
			dsec = (flag == SUNSET) ? SECS_IN_DAY : 0;
		}
 /*
  * 24 hrs darkness: set answer to 00:00:00
  */
		else {
			dsec = 0;
		}
	}
 /*
  * Find time in seconds-of-day.
  */
	else {
		dsec = zerobr(a, b, TOLERANCE, sunhorz);
		if (errno) {
			return (NULL);
		}
	}

 /*
  * add answer (time of day in seconds) to midnite of that day
  */
	tp->hour = 0;
	add_to_dt(tp, 0, (int) (dsec + 0.5));

	return (tp);
}

/* ------------------------------------------------------------------------ */

 /*
  * Returns cosine of solar zenith angle, as function of time-of-day in
  * seconds.  Cosine is negative if sun below horizon.
  *
  * Accesses global variables:
  *	dt_midnite
  *	xlat
  *	xlon
  */

static double
sunhorz(
	double          tsec)
{
	double          azm;		/* solar azimuth (not used)	 */
	double          cz;		/* solar cosine			 */
	datetime_t      tp;		/* local copy of date and time   */

 /*
  * add time-of-day in seconds to midnite of reference day
  */
	tp = dt_midnite;
	add_to_dt(&tp, 0, (int) (tsec + 0.5));

 /*
  * compute cosine of solar zenith angle, 0.0 at sunrise/set
  */
	if (sunangle(xlat, xlon, &tp, &cz, &azm, NULL) == ERROR) {
		errno = ERROR;
		return (HUGE_VAL);
	}

	return (cz);
}
