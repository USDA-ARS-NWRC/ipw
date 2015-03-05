/*
** NAME
**	now_dt -- get the current date and time
**
** SYNOPSIS
**	#include "ipw.h"
**
**	datetime_t *
**	now_dt(void)
**
** DESCRIPTION
**	now_dt creates a new date-time structure, and fills its fields
**	in with the current date and time.
**
** RETURN VALUE
**	non-NULL	pointer to new date-time structure
**
**	NULL		error occurred, and message stored via 'usrerr'
*/

#include <sys/types.h>
#include <time.h>
#include <errno.h>

#include "ipw.h"

datetime_t *
now_dt(void)
{
	time_t		clock;
	struct tm      *current_tm;
	datetime_t     *dt;

	if (time(&clock) == (time_t)-1) {
	    syserr();
	    usrerr("Cannot get current date and time from system");
	    return NULL;
	}

	current_tm = localtime(&clock);
	if (current_tm == NULL) {
	    if (errno != 0)
		syserr();
	    usrerr("Cannot convert current date and time from system");
	    return NULL;
	}

	dt = (datetime_t *) ecalloc(1, sizeof(datetime_t));
	if (dt == NULL) {
		usrerr("insufficient memory");
		return NULL;
	}

	dt->year = current_tm->tm_year + 1900;
	dt->month = current_tm->tm_mon + 1;
	dt->day = current_tm->tm_mday;
	dt->hour = current_tm->tm_hour;
	dt->min = current_tm->tm_min;
	dt->sec = current_tm->tm_sec;

	return dt;
}
