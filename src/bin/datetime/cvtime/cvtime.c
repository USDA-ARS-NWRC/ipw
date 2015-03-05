/*
** NAME
**	cvtime -- read input file and perform requested time conversion
**		  for command cvtime
** 
** SYNOPSIS
**	bool_t
**	cvtime(
**	    int		in_fmt_type,	|* type of input format		 *|
**	    char       *in_fmt,		|* input format string		 *|
**	    int		out_fmt_type,	|* type of output format	 *|
**	    char       *out_fmt,	|* output format string		 *|
**	    int		yr,		|* year				 *|
**	    bool_t	local_in,	|* flag for local time to GMT	 *|
**	    bool_t	local_out,	|* flag for GMT to local time	 *|
**	    int		zone,		|* minutes from Greenwich (+w,-e)*|
**	    bool_t	hrs,		|* flag for time in hours	 *|
**	    bool_t	secs,		|* flag for time in seconds	 *|
**	    char	*str)		|* string with date-time	 *|
** 
** DESCRIPTION
**	cvtime reads the standard input and performs the time conversions
**	based on the given input and output formats.
**
** RETURN VALUE
**	TRUE	End-of-file reached
**
**	FALSE	Not end-of-file.
*/

#include <unistd.h>
#include <math.h>	/* for floor() */

#include "ipw.h"
#include "pgm.h"

#define MAX_YR		5000
#define MAX_MON_YR	12
#define MAX_DAY_MON	31
#define MAX_HR_DAY	23
#define MAX_MIN_HR	59
#define MAX_SEC_MIN	59

bool_t
cvtime (
	int		in_fmt_type,	/* type of input format		 */
	char	       *in_fmt,		/* input format string		 */
	int		out_fmt_type,	/* type of output format	 */
	char	       *out_fmt,	/* output format string		 */
	int		yr,		/* year				 */
	bool_t		local_in,	/* flag for local time to GMT	 */
	bool_t		local_out,	/* flag for GMT to local time	 */
	int		zone,		/* minutes from Greenwich (+w,-e)*/
	bool_t		hrs,		/* flag for time in hours	 */
	bool_t		secs,		/* flag for time in seconds	 */
	char		*str)		/* string with date-time	 */
{
	datetime_t	dt;		/* date-time structure		 */
	double		jd;		/* Julian decimal day 		 */
	int		yday;		/* yearday (Jan 1 = day 1)	 */
	double		wyd;		/* water year decimal day	 */
	int		wday;		/* waterday (Oct 1 = day 1)	 */
	int		wy;		/* water year			 */
	int		total_secs;	/* time as total seconds	 */
	int		nread;		/* return from scanf		 */
	char		buf[MAX_INPUT]; /* buffer for output 		 */


	dt.year = yr;

	/* read from string and put into a date-time structure */

	switch (in_fmt_type) {

	    case JD:
	    case JD1:
		if (sscanf(str, "%lf", &jd) == 1) {
		    /*
		     *  Convert to julian decimal days.
		     */
		    if (secs)
			jd = SEC_TO_DAY(jd);
		    else if (hrs)
			jd = SEC_TO_DAY(HR_TO_SEC(jd));

		    /*
		     *  Convert julian decimal days into date-time.
		     */
		    yday = (int) floor(jd);
		    total_secs = DAY_TO_SEC(jd - yday);
		    if (in_fmt_type == JD) {
			yday++;
		    }
		    yday2mday(yr, yday, &(dt.month), &(dt.day));
		    sec2hms(total_secs, &(dt.hour), &(dt.min), &(dt.sec));
		} else {
		    return TRUE;
		}
		break;

	    case WY:
	    case WY1:
		if (sscanf(str, "%lf", &wyd) == 1) {
		    /*
		     *  Convert to waterday decimal days.
		     */
		    if (secs)
			wyd = SEC_TO_DAY(wyd);
		    else if (hrs)
			wyd = SEC_TO_DAY(HR_TO_SEC(wyd));
	
		    /*
		     *  Convert waterday decimal days into date-time.
		     */
		    wday = (int) floor(wyd);
		    total_secs = DAY_TO_SEC(wyd - wday);
		    if (in_fmt_type == WY) {
			wday++;
		    }
		    wday2mday(yr, wday, &(dt.year), &(dt.month), &(dt.day));
		    sec2hms(total_secs, &(dt.hour), &(dt.min), &(dt.sec));
		} else {
		    return TRUE;
		}
		break;

	    case DATE:
		if ((nread = sscanf(str, "%d,%d,%d,%d,%d,%d", &(dt.year),
				    &(dt.month), &(dt.day), &(dt.hour),
				    &(dt.min), &(dt.sec)) ) != EOF) {
		    if (nread != 6)
			error("bad or incomplete date in input");

		    /* check for bad input values */
		    if (dt.year<0 || dt.year>MAX_YR) {
			error("bad input year %d", dt.year);
		    }
		    if (dt.month<1 || dt.month>MAX_MON_YR) {
			error("bad input month %d", dt.month);
		    }
		    if (dt.day<1 || dt.day>MAX_DAY_MON) {
			error("bad input day %d", dt.day);
		    }
		    if (dt.hour<0 || dt.hour>MAX_HR_DAY) {
			error("bad input hour %d", dt.hour);
		    }
		    if (dt.min<0 || dt.min>MAX_MIN_HR) {
			error("bad input minute %d", dt.min);
		    }
		    if (dt.sec<0 || dt.sec>MAX_SEC_MIN) {
			error("bad input second %d", dt.sec);
		    }
		} else {
		    return TRUE;
		}
		break;

	    case FMTSTR:
		if (! str2dt(str, in_fmt, &dt)) {
		    error("Error reading date and time");
		}
		break;
	} /* switch on input format type */


	/* do time zone conversion, if necessary */

	if (local_in)
	    local2gmt(&dt, zone, 0);

	if (local_out)
	    gmt2local(&dt, zone, 0);

	/* convert date-time to output format and print */

	switch (out_fmt_type) {

	    case JD:
	    case JD1:
		/*
		 *  Convert year, month, day -> yearday
		 */
		yday = yearday(dt.year, dt.month, dt.day);
		if (out_fmt_type == JD)
		    yday--;

		/*
		 *  Add time as decimal day to yearday
		 */
		jd = yday + SEC_TO_DAY(HMS_TO_SEC(dt.hour, dt.min, dt.sec));

		if (secs)
		    printf ("%f\n", DAY_TO_SEC(jd));
		else if (hrs)
		    printf ("%f\n", SEC_TO_HR(DAY_TO_SEC(jd)));
		else
		    printf ("%f\n", jd);
		break;

	    case WY:
	    case WY1:
		/*
		 *  Convert year, month, day -> waterday
		 */
		wday = waterday(dt.year, dt.month, dt.day, &wy);
		if (out_fmt_type == WY)
		    wday--;

		/*
		 *  Add time as decimal day to waterday
		 */
		wyd = wday + SEC_TO_DAY(HMS_TO_SEC(dt.hour, dt.min, dt.sec));

		if (secs)
		    printf ("%f\n", DAY_TO_SEC(wyd));
		else if (hrs)
		    printf ("%f\n", SEC_TO_HR(DAY_TO_SEC(wyd)));
		else
		    printf ("%f\n", wyd);
		break;


	    case DATE:
		printf ("%d,%d,%d,%d,%d,%d\n", dt.year, dt.month, dt.day,
			dt.hour, dt.min, dt.sec);
		break;

	    case FMTSTR:
		if (dt2fmtstr(&dt, out_fmt, buf)) {
		    printf("%s\n", buf);
		} else {
		    error("Cannot format date-time for output");
		}
		break;
	}  /* switch output format type */

	return FALSE;
}
