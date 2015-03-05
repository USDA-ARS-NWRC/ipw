/*
** NAME
**	wday2mday -- converts a waterday into a month and day
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	wday2mday(
**		int 	wateryear,	|* e.g., 1997			      *|
**		int     waterday,	|* [1 - 366]			      *|
**		int   * year,		|* output: [wateryear or wateryear-1] *|
**		int   * month,		|* output: [1 - 12]		      *|
**		int   * day)		|* output: [1 - 31]		      *|
**
** DESCRIPTION
**	wday2mday converts a waterday and wateryear into a month, day,
**	and year.
*/

#include "ipw.h"

void
wday2mday(
	int             wateryear,	/* e.g., 1997			      */
	int             waterday,	/* [1 - 366]			      */
	int           * year,		/* output: [wateryear or wateryear-1] */
	int           * month,		/* output: [1 - 12]		      */
	int           * day)		/* output: [1 - 31]		      */
{
	int		ndays;		/* # of days in month	*/

	assert(wateryear >= 0);
	/*
	 *  Because a water year ends the calendar year of the same number,
	 *  a water year is a leap year when it's corresponding calendar year
	 *  is a leapyear.  Therefore, the macro "DAYS_IN_YR" below works
	 *  correctly.
	 */
	assert(1 <= waterday && waterday <= DAYS_IN_YR(wateryear));
	assert(year != NULL);
	assert(month != NULL);
	assert(day != NULL);

	/*
	 *  If the waterday is within the first 92 days, then the month
	 *  and day are between Oct 1 and Dec 31 of the previous calendar
	 *  year.
	 */
	if (waterday <= 92) {
		*year = wateryear - 1;
		*month = 10;
		ndays = numdays(*year, *month);
		*day = waterday;
		while (*day > ndays) {
			*day -= ndays;
			(*month)++;
			ndays = numdays(*year, *month);
		}
	}

	/*
	 *  Otherwise, the waterday is between Jan 1 and Sep 30 of this
	 *  calendar year.  Subtract the first 92 days off to yield
	 *  yearday, then convert to month and day.
	 */
	else {
		*year = wateryear;
		yday2mday(wateryear, waterday - 92, month, day);
	}
}
