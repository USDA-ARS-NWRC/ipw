/*
** NAME
**	yearday -- returns the yearday (day of year) for a given date
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	yearday(
**		int 	year,		|* e.g., 1997		*|
**		int 	month,		|* [1 - 12]		*|
**		int 	day)		|* [1 - 31]		*|
**
** DESCRIPTION
**	yearday returns the yearday for the given date.  yearday is
**	the 'day of the year', sometimes called (incorrectly) 'julian day'.
**
** RETURN VALUE
**	1 - 366
*/

#include "ipw.h"

int
yearday(
	int             year,		/* e.g., 1997		*/
	int             month,		/* [1 - 12]		*/
	int             day)		/* [1 - 31]		*/
{
	int		yday;		/* yearday		*/

	assert(year >= 0);
	assert(1 <= month && month <= 12);
	assert(1 <= day && day <= numdays(year, month));

	yday = day;

	/*
	 *  Add in number of days for preceeding months.
	 */
	for (month--; month > 0; month--)
		yday += numdays(year, month);

	return yday;
}
