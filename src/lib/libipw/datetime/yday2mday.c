/*
** NAME
**	yday2mday -- convert yearday into month and day
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	yday2mday(
**		int 	year,		|* e.g., 1997		*|
**		int     yrday,		|* yearday [1 - 366]	*|
**		int   * month,		|* output: [1 - 12]	*|
**		int   * day)		|* output: [1 - 31]	*|
**
** DESCRIPTION
**	yday2mday converts a given yearday into the month and day
**	for the given year.
*/

#include "ipw.h"

void
yday2mday(
	int             year,		/* e.g., 1997		*/
	int             yrday,		/* yearday [1 - 366]	*/
	int           * month,		/* output: [1 - 12]	*/
	int           * day)		/* output: [1 - 31]	*/
{
	int		ndays;		/* # of days in month	*/

	assert(year >= 0);
	assert(1 <= yrday && yrday <= DAYS_IN_YR(year));
	assert(month != NULL);
	assert(day != NULL);

	/*
	 *  Go thru months subtract # of days in the month from yearday
	 *  until yearday is within current month.
	 */
	*month = 1;
	ndays = numdays(year, *month);
	while (yrday > ndays) {
		yrday -= ndays;
		(*month)++;
		ndays = numdays(year, *month);
	}
	*day = yrday;
}
