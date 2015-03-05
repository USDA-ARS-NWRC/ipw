#include "ipw.h"

void
dt_diff(
	datetime_t     *dt1,	   /* first date-time		       */
	datetime_t     *dt2,	   /* second date-time		       */

     /* output variables */

	int	       *days,	   /* # of days between dt1 and dt2    */
	int	       *seconds)   /* # of seconds between dt1 and dt2 */
{
	datetime_t     *start_dt;   /* earlier of two date-times       */
	datetime_t     *end_dt;	    /* later   "   "    "    "	       */
	bool_t		dt2_b4_dt1; /* is 2nd dt earlier than 1st dt1? */
	int		year;	    /* loop index	 */
	int		start_tod;  /* start_dt's time-of-day (seconds)*/
	int		end_tod;    /* end_dt's     "  "   "  (seconds)*/

	assert(dt1 != NULL);
	assert(dt2 != NULL);
	assert(days != NULL);
	assert(seconds != NULL);

	/*
	 *  Determine which day is earlier.
	 */
	if (dt_in_order(dt1, dt2)) {
		start_dt = dt1;
		end_dt   = dt2;
		dt2_b4_dt1 = FALSE;
	} else {
		start_dt = dt2;
		end_dt   = dt1;
		dt2_b4_dt1 = TRUE;
	}

	/*
	 *  Determine the number of days from start day to end day.
	 */
	if (start_dt->year == end_dt->year) {
		*days = yearday(end_dt->year, end_dt->month, end_dt->day) -
			yearday(start_dt->year, start_dt->month, start_dt->day);
	}
	else {
		/*
		 *  Assume: start_dt->year must be < end_dt->year
		 *
		 *  So # of days = # of days from start day to end of start yr
		 *		   + # of days in years between start & end yrs
		 *		   + yearday of end day
		 */
		*days = DAYS_IN_YR(start_dt->year) -
			yearday(start_dt->year, start_dt->month, start_dt->day);
		for (year = start_dt->year + 1; year < end_dt->year; year++)
			*days += DAYS_IN_YR(year);
		*days += yearday(end_dt->year, end_dt->month, end_dt->day);
	}

	/*
	 *  Now, determine the number of seconds from the start time-of-day to
	 *  the end time-of-day.
	 *
	 *  Note: the end time-of-day may be earlier in the day than the
	 *	  start time-of-day.
	 */
	start_tod = HMS_TO_SEC(start_dt->hour, start_dt->min, start_dt->sec);
	end_tod   = HMS_TO_SEC(end_dt->hour, end_dt->min, end_dt->sec);
	if (start_tod <= end_tod) {
		*seconds = end_tod - start_tod;
	} else {
		*seconds = HR_TO_SEC(24) + (end_tod - start_tod);
		(*days) --;
	}

	/*
	 *  Now if the second date-time was earlier than the first date-time,
	 *  negate the answers.
	 */
	if (dt2_b4_dt1) {
		*days = - (*days);
		*seconds = - (*seconds);
	}
}
