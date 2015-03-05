/*
** NAME
**	sec2hms -- converts total seconds into hours, minutes, seconds
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	sec2hms(
**	    int      total_secs,	|* total time as seconds	*|
**	    int    * hrs,		|* output: [0 - ...		*|
**	    int    * mins,		|* output: [1 - 59]		*|
**	    int    * secs)		|* output: [1 - 59]		*|
**
** DESCRIPTION
**	sec2hms converts a time expressed as total number of seconds
**	as hours, minutes, and seconds.
*/

#include "ipw.h"

void
sec2hms(
	int             total_secs,	/* total time as seconds	*/
	int           * hrs,		/* output: [0 - ...		*/
	int           * mins,		/* output: [1 - 59]		*/
	int           * secs)		/* output: [1 - 59]		*/
{
	int		total_mins;

	assert(total_secs >= 0);
	assert(hrs != NULL);
	assert(mins != NULL);
	assert(secs != NULL);

	*secs = total_secs % 60;
	total_secs -= *secs;

	total_mins = SEC_TO_MIN(total_secs);
	*mins = total_mins % 60;
	total_mins -= *mins;

	*hrs = MIN_TO_HR(total_mins);
}
