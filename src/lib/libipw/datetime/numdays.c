/*
** NAME
** 	numdays - return number of days in a month
** 
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	numdays(
**		int		year,		|* year			*|
**		int		month)		|* month (1 to 12)	*|
** 
** DESCRIPTION
**	numdays retuns the number of days in the given month of
**	the given year.
** 
** RETURN VALUE
**	number of days in month
*/

#include "ipw.h"

int
numdays(
	int		year,		/* year			*/
	int		month)		/* month (1 to 12)	*/
{
	static int NDAYS[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int ndays;

	assert(year >= 0);
	assert(1 <= month && month <= 12);

	ndays = NDAYS[month-1];

   /*
    *  Check for leap year for February
    */
	if ((month == 2) && leapyear(year)) {
		ndays++;
	}

	return (ndays);
}
