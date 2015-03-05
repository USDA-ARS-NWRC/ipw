/*
** NAME
** 	add_to_dt - add given # of days and seconds to date-time
** 
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	add_to_dt(
**		datetime_t     *dt,	   |* date-time			 *|
**		int		days,	   |* # of days to add		 *|
**		int		seconds)   |* # of seconds to add	 *|
** 
** DESCRIPTION
**	add_to_dt adds a given number of days and seconds to a given
**	date and time.  Either or both of the number of days and seconds can
**	be 0 or negative.
*/

#include "ipw.h"

void
add_to_dt(
	datetime_t     *dt,		/* date-time			 */
	int		days,		/* # of days to add		 */
	int		seconds)	/* # of seconds to add		 */
{
	int		time_of_day;	/* in seconds			 */
	int		yrday;		/* yearday			 */
	int		yearlen;	/* length of year in days	 */

	assert (dt != NULL);

	/*
	 *  Determine the new time of day.
	 */
	time_of_day = HMS_TO_SEC(dt->hour, dt->min, dt->sec);
	if (seconds > 0) {
		time_of_day += seconds;
		days += time_of_day / (int) HR_TO_SEC(24);
		time_of_day %= (int) HR_TO_SEC(24);
		sec2hms(time_of_day, &(dt->hour), &(dt->min), &(dt->sec));
	} else if (seconds < 0) {
		time_of_day += seconds;
		while (time_of_day < 0) {
			time_of_day += HR_TO_SEC(24);
			days--;
		}
		sec2hms(time_of_day, &(dt->hour), &(dt->min), &(dt->sec));
	}

	/*
	 *  Determine the new date.
	 */
	yrday = yearday(dt->year, dt->month, dt->day);
	if (days > 0) {
		yrday += days;
		yearlen = DAYS_IN_YR(dt->year);
		while (yrday > yearlen) {
			yrday -= yearlen;
			dt->year++;
			yearlen = DAYS_IN_YR(dt->year);
		}
		yday2mday(dt->year, yrday, &(dt->month), &(dt->day));
	} else if (days < 0) {
		yrday += days;
		while (yrday < 1) {
			dt->year--;
			yrday += DAYS_IN_YR(dt->year);
		}
		yday2mday(dt->year, yrday, &(dt->month), &(dt->day));
	}
}
