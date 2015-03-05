/*
** NAME
**	make_dt -- create a new date-time structure
**
** SYNOPSIS
**	#include "ipw.h"
**
**	datetime_t *
**	make_dt(
**		int             year,		|* e.g., 1997		*|
**		int             month,		|* [1 - 12]		*|
**		int             day,		|* [1 - 31]		*|
**		int             hour,		|* [0 - 23]		*|
**		int             min,		|* [0 - 59]		*|
**		int             sec)		|* [0 - 59]		*|
**
** DESCRIPTION
**	make_dt creates a new date-time structure, and fills its fields
**	in with the given date and time values.  It also checks to see
**	if the given values are valid.
**
** RETURN VALUE
**	non-NULL	pointer to new date-time structure
**
**	NULL		error occurred, and message stored via 'usrerr'
*/

#include "ipw.h"

datetime_t *
make_dt(
	int             year,		/* e.g., 1997		*/
	int             month,		/* [1 - 12]		*/
	int             day,		/* [1 - 31]		*/
	int             hour,		/* [0 - 23]		*/
	int             min,		/* [0 - 59]		*/
	int             sec)		/* [0 - 59]		*/
{
	datetime_t     *dt;

	if (year < 0) {
		usrerr("year (%d) isn't 0 or positive", year);
		return NULL;
	}
	if (month < 1 || month > 12) {
		usrerr("month (%d) isn't between 1 and 12", month);
		return NULL;
	}
	if (day < 1 || day > numdays(year,month)) {
		usrerr("day (%d) isn't between 1 and last day of month (%d)",
			day, numdays(year, month));
		return NULL;
	}

	if (hour < 0 || hour > 23) {
		usrerr("hours (%d) isn't between 0 and 23", hour);
		return NULL;
	}
	if (min < 0 || min > 59) {
		usrerr("minutes (%d) isn't between 0 and 59", min);
		return NULL;
	}
	if (sec < 0 || sec > 59) {
		usrerr("seconds (%d) isn't between 0 and 59", sec);
		return NULL;
	}

	dt = (datetime_t *) ecalloc(1, sizeof(datetime_t));
	if (dt == NULL) {
		usrerr("insufficient memory");
		return NULL;
	}

	dt->year = year;
	dt->month = month;
	dt->day = day;
	dt->hour = hour;
	dt->min = min;
	dt->sec = sec;

	return dt;
}
