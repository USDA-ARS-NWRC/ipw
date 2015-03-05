/*
** NAME
** 	gmt2local - convert a gmt (UTC) date-time into local date-time
** 
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	gmt2local(
**		datetime_t     *dt,	|* GMT date-time		 *|
**		int		zone,	|* local time zone (minutes of
**					   time west of Greenwich)       *|
**		bool_t		isDST)	|* daylight savings time?	 *|
** 
** DESCRIPTION
**	gmt2local converts a GMT which is also known as Coordinated
**	Universal Time (UTC), date-time into local time.  The
**	local time zone is specified by the number of minutes of time
**	west of Greenwich.  isDST indicates where or not daylight savings
**	time is in effect for the local time zone.
*/

#include "ipw.h"

void
gmt2local(
	datetime_t     *dt,		/* GMT date-time		 */
	int		zone,		/* local time zone (minutes of
					   time west of Greenwich)       */
	bool_t		isDST)		/* daylight savings time?	 */
{
	assert (dt != NULL);

	/*
	 *  Adjust the date-time by 0 days and -X seconds where X is zone
	 *  converted to seconds minus the daylight savings hour as seconds.
	 */
	add_to_dt(dt, 0, -(MIN_TO_SEC(zone) - (isDST ? HR_TO_SEC(1) : 0)) );
}
