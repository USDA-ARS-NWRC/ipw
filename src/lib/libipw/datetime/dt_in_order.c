#include "ipw.h"

bool_t
dt_in_order(
	datetime_t     *dt1,	   /* first date-time		       */
	datetime_t     *dt2)	   /* second date-time		       */
{
	assert(dt1 != NULL);
	assert(dt2 != NULL);

	if (dt1->year < dt2->year)
		return TRUE;
	if (dt1->year > dt2->year)
		return FALSE;

	/*
	 *  dt1's year == dt2's year, so check months
	 */
	if (dt1->month < dt2->month)
		return TRUE;
	if (dt1->month > dt2->month)
		return FALSE;

	/*
	 *  dt1's month == dt2's month, so check days
	 */
	if (dt1->day < dt2->day)
		return TRUE;
	if (dt1->day > dt2->day)
		return FALSE;


	/*
	 *  dt1's day == dt2's day, so check hours
	 */
	if (dt1->hour < dt2->hour)
		return TRUE;
	if (dt1->hour > dt2->hour)
		return FALSE;


	/*
	 *  dt1's hour == dt2's hour, so check minutes
	 */
	if (dt1->min < dt2->min)
		return TRUE;
	if (dt1->min > dt2->min)
		return FALSE;


	/*
	 *  dt1's minutes == dt2's minutes, so check seconds
	 */
	return (dt1->sec <= dt2->sec);
}
