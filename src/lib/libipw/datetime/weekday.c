/*
** NAME
**	weekday -- returns the day of the week for a given date
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	weekday(
**		int 	year,		|* e.g., 1997		*|
**		int 	month,		|* [1 - 12]		*|
**		int 	day)		|* [1 - 31]		*|
**
** DESCRIPTION
**	weekday returns the day of the week for a given date as an
**	integer between 1 (Sunday) and 7 (Saturday).
**
** RETURN VALUE
**	1 - 7
*/

#include "ipw.h"

int
weekday(
	int             year,		/* e.g., 1997		*/
	int             month,		/* [1 - 12]		*/
	int             day)		/* [1 - 31]		*/
{
	int		days;		/* # of days between dt and ref dt */
	int		wkday;

	assert(year >= 0);
	assert(1 <= month && month <= 12);
	assert(1 <= day && day <= numdays(year, month));

	/*
	 *  Our reference is January 1, 1970 (Thursday).
	 *  Determine the number of days between given date and this reference
	 *  date.
	 */
	if (year >= 1970) {
		days = yearday(year, month, day) - 1;
		year--;
		while (year >= 1970) {
			days += DAYS_IN_YR(year);
			year--;
		}
		wkday = ((4 + days) % 7) + 1;
	} else {
		/*
		 *  Days from current date to start of next year.
		 */
		days = DAYS_IN_YR(year) - yearday(year, month, day) + 1;
		year++;
		while (year < 1970) {
			days += DAYS_IN_YR(year);
			year++;
		}
		wkday = 4 - (days % 7);
		if (wkday < 0)
		 	wkday += 7;
		wkday++;  /* convert from 0 .. 6 range to 1 .. 7 range */
	}

	return wkday;
}
