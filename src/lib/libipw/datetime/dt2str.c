/*
** NAME
** 	dt2str -- convert a local date-time into a string
** 
** SYNOPSIS
**	#include "ipw.h"
**
**	char *
**	dt2str(
**		datetime_t     *dt)		|* date-time		 *|
** 
** DESCRIPTION
**	dt2str converts a date-time value into a 25-character string
**	formatted as follows:
**
**		Sun Sep 16 01:03:52 1973\n
**
**	This is the same format that the system call 'asctime' yields
**	for a tm structure.  Like asctime, this function returns a
**	pointer to static memory which is overwritten each time this
**	this function is called.
*/

#include "ipw.h"

static char * monthnames[] = {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
	};

static char * weekdays[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
	};

static char buffer[26];

char *
dt2str(
	datetime_t     *dt)		/* date-time		 */
{
	assert (dt != NULL);

	sprintf(buffer, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
			weekdays[weekday(dt->year, dt->month, dt->day) - 1],
			monthnames[dt->month - 1], dt->day, dt->hour, dt->min,
			dt->sec, dt->year);
	return buffer;
}
