/*
** NAME
**	leapyear -- is a given year a leap year?
**
** SYNOPSIS
**	#include "ipw.h"
**
**	bool_t
**	leapyear(
**		int 	year)
**
** DESCRIPTION
**	leapyear determines if the given year is a leap year or not.
**	year must be positive, and must not be abbreviated; i.e.
**	89 is 89 A.D. not 1989.
**
** RETURN VALUE
**	TRUE	if a leap year
**
**	FALSE	if not a leap year
*/

#include "ipw.h"

bool_t
leapyear(
	int             year)
{
	assert(year >= 0);

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}
