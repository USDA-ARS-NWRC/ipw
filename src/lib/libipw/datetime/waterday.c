/*
** NAME
**	waterday -- returns day of water year for a given date
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	waterday(
**		int 	year,		|* e.g., 1997			*|
**		int 	month,		|* [1 - 12]			*|
**		int 	day)		|* [1 - 31]			*|
**		int    *wateryear)	|* output [ year or year + 1 ]	*|
**
** DESCRIPTION
**	waterday returns the day of the water year for the given date.
**	The "water year" runs from October 1 to September 30.  Water years
**	are designated by the year they *end* in.  For example, the 1997
**	water year runs from Oct 1, 1996 to September 30, 1997.
**	The output parameter 'wateryear' is assigned the water year for
**	the given date.
**
** RETURN VALUE
**	1 - 366
*/

#include "ipw.h"

int
waterday(
	int             year,		/* e.g., 1997			*/
	int             month,		/* [1 - 12]			*/
	int             day,		/* [1 - 31]			*/
	int	       *wateryear)	/* output [ year or year + 1 ]	*/
{
	int		wday;		/* water day		*/

	assert(year >= 0);
	assert(1 <= month && month <= 12);
	assert(1 <= day && day <= numdays(year, month));
	assert(wateryear != NULL);

	/*
	 *  If we're on or past October 1st, then the water day is just
	 *  the number of days since October 1st, and the water year
	 *  is *next* year.
	 */
	if (month >= 10) {
		wday = day;
		for (month--; month >= 10; month--) {
			wday += numdays(year, month);
		}
		*wateryear = year + 1;
	}

	/*
	 *  Otherwise, before October 1st, so water day is yearday plus
	 *  the number of days from Oct 1 of last year to Dec 31 of last
	 *  year (= 92),  and water year is this year.
	 */
	else {
		wday = yearday(year, month, day) + 92;
		*wateryear = year;
	}

	return wday;
}
