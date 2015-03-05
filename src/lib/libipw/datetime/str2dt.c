/*
 Function scans a string looking for a date and time in a specified format:
 The format is specfied as a string containing format codes.  A format
 code denotes a particular element in the date or time and the format
 the function expects the element to be in.

  for a month, a) look for 2-digits
		b) look for 1 or 2 digits
  1998/1/4
  1998/01/04
  98/1/04
  98/01/04

 Acceptable format codes:
   %y   --  year within century (2 digits)  (default for century ?)
   %Y   --    complete year (4 digits)
   %m   --  month as 1 or 2 digits (e.g., 1, 12, 01)
   %d   --  day as 1 or 2 digits (0 padding acceptable)
   %wy  --  water year within century (2 digits) (default for century ?)
   %wY  --    complete wateryear (4 digits)
   %j   --  yearday as 1, 2 or 3 digits
   %Jd  --  Julian day (decimal days; Oct 1, midnight = 0.0)
   %J1  --  Julian day (decimal days; Oct 1, midnight = 1.0;
                        hence "jd1" is equivalent to "jd"+1)
   %wd  --  waterday as 1, 2 or 3 digits (0 padding ok)
   %Wd  --  wateryear day (decimal days; Oct 1, midnight = 0.0)
   %W1  --  wateryear day (decimal days; Oct 1, midnight = 1.0;
                           hence "wyd1" is equivalent to "wyd"+1)

   %H   --  hour of day as number (1 or 2 digits: 0-23)
   %h   --    hour of day as 2 digits (12-hour clock)
   %M   --  minute of day as 2 digits
   %S   --  seconds of day as 2 digits
   %a   --  "am" or "pm"
   %A   --  "AM" or "PM"

   %%   --  "%"
   %?   --  any old character
   %+   --  1 or more whitespace characters
   %*   --  0 or more whitespace characters

   any other character must match exactly

if no %S, assume 0
if no %M, assume 0
if no %H, assume 0 (midnite)
if no year, i.e., no %wy, %wY, %y, or %Y, assume current year
if for %wy and %Y, assume century is current century
if no day, assume first of month
if no month, assume first month ()

*/

#include "ipw.h"
#include <math.h>

/* ----------------------------------------------------------------------- */

/*
 *  List of format codes that are not compatible with all other codes.
 */

char *fcode_names[] = {
#define FCODE_y		0
	"y",
#define FCODE_Y		1
	"Y",
#define FCODE_m		2
	"m",
#define FCODE_d		3
	"d",
#define FCODE_wd	4
	"wd",
#define FCODE_wy	5
	"wy",
#define FCODE_wY	6
	"wY",
#define FCODE_j		7
	"j",
#define FCODE_H		8
	"H",
#define FCODE_M		9
	"M",
#define FCODE_S		10
	"S",
#define FCODE_h		11
	"h",
#define FCODE_a		12
	"a",
#define FCODE_A		13
	"A",
#define FCODE_Jd	14
	"Jd",
#define FCODE_J1	15
	"J1",
#define FCODE_Wd	16
	"Wd",
#define FCODE_W1	17
	"W1"
};

#define NUM_FCODES	18

/*
 *  List of format codes found in the format string
 */
static int fcode_cnt;
static int fcodes_found[NUM_FCODES];

/*
 *  Compatibility chart indicating which format codes can be used with
 *  each other. 'D' means duplicated format code, which is an error
 */

static short compatibility_table[NUM_FCODES][NUM_FCODES] = {

  /*	     y   Y   m   d  wd  wy  wY   j   H   M   S   h   a   A  Jd  J1  Wd   W1  */

  /* y  */  -1,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* Y  */   0, -1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* m  */   1,  1, -1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* d  */   1,  1,  1, -1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* wd */   0,  0,  0,  0, -1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* wy */   0,  0,  0,  0,  1, -1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* wY */   0,  0,  0,  0,  1,  0, -1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* j  */   1,  1,  0,  0,  0,  0,  0, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* H  */   1,  1,  1,  1,  1,  1,  1,  1, -1,  1,  1,  0,  0,  0,  1,  1,  1,  1,
  /* M  */   1,  1,  1,  1,  1,  1,  1,  1,  1, -1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* S  */   1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1,  1,  1,  1,  1,  1,  1,  1,
  /* h  */   1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1, -1,  1,  1,  1,  1,  1,  1,
  /* a  */   1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1, -1,  0,  1,  1,  1,  1,
  /* A  */   1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  0, -1,  1,  1,  1,  1,
  /* Jd */   1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0,  0,
  /* J1 */   1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0,
  /* Wd */   0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,
  /* W1 */   0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1
};

/* ----------------------------------------------------------------------- */

/*
 *  The function determines if the current format code that's been found
 *  in the format string is compatible with the previously encountered
 *  format codes.
 */

static bool_t
fcode_compatible(
	int	fcode)		/* current format code encountered	*/
{
	int	i;

	for (i = 0; i < fcode_cnt; i++) {
	    switch (compatibility_table[fcodes_found[i]][fcode]) {
		case 0 :
		    usrerr("Incompatible format codes: '%s' and '%s'",
			   fcode_names[fcodes_found[i]], fcode_names[fcode]);
		    return FALSE;

		case -1 :
		    usrerr("Format code appears more than once: '%s'",
			    fcode_names[fcode]);
		    return FALSE;

		default :
		    /* Compatibile */
		    break;
	    }
	}

	/*
	 *  Add current code to list of those found.
	 */
	fcodes_found[i] = fcode;
	fcode_cnt++;

	return TRUE;
}

/* ----------------------------------------------------------------------- */

/*
 *  This function reads a double from a string.  The function will read
 *  digits for the double until a non-digit is found. The function returns
 *  the # of digits read.  If at least one digit is not read, the
 *  function stores an error message using the "usrerr" function.
 */

static int
read_float(
	char	*str,		/* string to be scanned			*/
	char	*name,		/* name of integer for error message	*/
 /* output */
	double	*number)	/* the number read 			*/
{
	int	 digits = 0;	/* # of digits found			*/
	int	 day = 0;
	int	 hflag = 0;
	int	 hold = 0;
	int	 dig2 = 0;
	char	*s = str;	/* ptr for traversing string		*/
	double	 fnum;		/* hold for float			*/
	double	 hms;

	*number = 0;

	while ((isdigit(*s)) || (*s == '.')) {
	    if (*s == '.') {
		day = hold;
		hold = 0;
		hflag = TRUE;
		dig2 = 0;
		digits++;
		s++;
	    } else {
		hold = hold * 10 + (*s - '0');
		dig2++;
		digits++;
		s++;
	    }
	}
	/*
 	 *  convert to double
 	 */
	if (hflag) {
	    hms = ( ((double) hold) / (pow(10.0, (double) dig2)));
	} else {
	    hms = 0.0;
	    day = hold;
	}
	fnum = (double) day + hms;

	*number = fnum;

	return digits;
}

/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */

/*
 *  This function reads an integer from a string.  The function will read
 *  digits for the integer until a non-digit is found or until the maximum
 *  number of digits is reached.  The function returns the # of digits read.
 *  If the minimum # of digits is not read, the function stores an error
 *  message using the "usrerr" function.
 */

static int
read_int(
	char	*str,		/* string to be scanned			*/
	int	 min_digits,	/* minimum # of digits			*/
	int	 max_digits,	/* minimum # of digits			*/
	char	*name,		/* name of integer for error message	*/
 /* output */
	int     *integer)	/* the integer read 			*/
{
	int	 digits = 0;	/* # of digits found			*/
	char	*s = str;	/* ptr for traversing string		*/

	*integer = 0;

	while (isdigit(*s) && (digits < max_digits)) {
	    *integer = (*integer) * 10 + (*s - '0');
	    digits++;
	    s++;
	}

	/*
	 *  Did we get the minimum # of digits?
	 */
	if (digits < min_digits) {
	    if (digits == 0) {
		usrerr("Missing number for %s", name);
	    } else if (min_digits == max_digits) {
		usrerr("Found \"%.*s\" for %s instead of %d-digit number",
			digits+1, str, name, min_digits);
	    } else {
		usrerr("Found \"%.*s\" for %s instead of number with %d to %d digits",
			digits+1, str, name, min_digits, max_digits);
	    }
	}

	return digits;
}

/* ----------------------------------------------------------------------- */

bool_t
str2dt(
	char            *str,		/* string to be scanned		*/
	char            *format,	/* string with format codes	*/

 /* output */
	datetime_t	*dt)		/* date-time scanned		*/
{
    /*
     *  defaults
     */
	int		 year;
	int		 month = 1;
	int		 day = 1;
	int		 yday = 1;
	int		 wyear;
	int		 wday = 1;
	int		 hours = 0;
	int		 minutes = 0;
	int		 seconds = 0;
	char		 am_pm = 'A';
	double		 jd = 0.0;
	double		 wyd = 0.0;

	datetime_t	*now;		/* current date and time	*/
	int		 century;	/* century of current year	*/
	int		 century_wy;	/* century of current wateryear */

	bool_t		 have_month_day = FALSE;
	bool_t		 have_yearday = FALSE;
	bool_t		 have_jday = FALSE;
	bool_t		 have_wyday = FALSE;
	bool_t		 have_waterday = FALSE;
	bool_t		 have_12hr_time = FALSE;

	int		 digits;	/* # of digits read 		*/
	int		 total_secs;


	assert(str != NULL);
	assert(format != NULL);
	assert(dt != NULL);

	/*
	 *  Initialize the list of found format codes to empty.
	 */
	fcode_cnt = 0;

	/*
	 *  Initialize year variables to current century.
	 */
	now = now_dt();
	if (now == NULL) {
	    return FALSE;
	}
	year = now->year;
	century = (now->year / 100) * 100;
	(void) waterday(now->year, now->month, now->day, &wyear);
	century_wy = (wyear / 100) * 100;

	/*
	 *  Scan through the format string along with the given string.
	 */
	while (*format != EOS) {
	    if (*format != '%') {
		/*
		 *  Not a format code, so just a character.  See if the
		 *  current character in the scan string is the same.
		 */
		if (*format == *str) {
		    format++;
		    str++;
		} else {
		    usrerr("Invalid character: '%c', expecting this: '%c'",
			   *str, *format);
		    return FALSE;
		}
	    } else {
		/*
		 *  Look for a format code.
		 */
		format++;
		switch (*format) {
		    case EOS :
			usrerr("Incomplete format code at end of format string");
			return FALSE;

		    case 'y' :
			if (! fcode_compatible(FCODE_y))
			    return FALSE;
			digits = read_int(str, 2, 2, "year", &year);
			if (digits < 2)
			    return FALSE;
			str += digits;
			year += century;
			break;

		    case 'Y' :
			if (! fcode_compatible(FCODE_Y))
			    return FALSE;
			digits = read_int(str, 1, 4, "year", &year);
			if (digits < 1)
			    return FALSE;
			str += digits;
			break;

		    case 'm' :
			if (! fcode_compatible(FCODE_m))
			    return FALSE;
			digits = read_int(str, 1, 2, "month", &month);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if ((month < 1) || (month > 12)) {
			    usrerr("Invalid month: %d", month);
			    return FALSE;
			}
			have_month_day = TRUE;
			break;

		    case 'd' :
			if (! fcode_compatible(FCODE_d))
			    return FALSE;
			digits = read_int(str, 1, 2, "day", &day);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if ((day < 1) || (day > 31)) {
			    usrerr("Invalid day: %d", day);
			    return FALSE;
			}
			have_month_day = TRUE;
			break;

		    case 'j' :
			if (! fcode_compatible(FCODE_j))
			    return FALSE;
			digits = read_int(str, 1, 3, "yearday", &yday);
			if (digits < 1)
			    return FALSE;
			str += digits;

			if ((yday < 1) || (yday > 366)) {
			    usrerr("Invalid yearday: %d", yday);
			    return FALSE;
			}

			have_yearday = TRUE;
			break;

	    	    case 'J':
			format++;
			switch (*format) {
			    case EOS :
				usrerr("Incomplete format code at end of format string");
				return FALSE;

			    case 'd' :
				if (! fcode_compatible(FCODE_Jd))
				    return FALSE;

				digits = read_float(str, "digital_yearday", &jd);
				if (digits < 1)
				    return FALSE;
				str += digits;

		    		/*
		     	 	 *  Convert julian decimal days into date-time.
		     	 	 */
		    		yday = (int) floor(jd);
		    		total_secs = DAY_TO_SEC(jd - yday);

				/*
				 * add 1 to wday for 0-start day adjustment
				 */
				yday++;

				if ((yday < 1) || (yday > 366)) {
				    usrerr("Invalid julian day: %d", yday);
				    return FALSE;
				}

				have_yearday = TRUE;
				have_jday = TRUE;
				break;

			    case '1':
				if (! fcode_compatible(FCODE_J1))
				    return FALSE;

				digits = read_float(str, "digital_yearday", &jd);
				if (digits < 1)
				    return FALSE;
				str += digits;

		    		/*
		     	 	 *  Convert julian decimal days into date-time.
		     	 	 */
		    		yday = (int) floor(jd);
		    		total_secs = DAY_TO_SEC(jd - yday);

				if ((yday < 1) || (yday > 366)) {
				    usrerr("Invalid julian day: %d", yday);
				    return FALSE;
				}

				have_yearday = TRUE;
				have_jday = TRUE;
				break;

			    default :
				usrerr("Unknown format code: %J%c", *format);
				return FALSE;
			} /* switch for %J format codes */
			break;

		    case 'W':
			format++;
			switch (*format) {
			    case EOS :
				usrerr("W-level - Incomplete format code at end of format string");
				return FALSE;
	
			    case 'd' :
				if (! fcode_compatible(FCODE_Wd))
				    return FALSE;

				digits = read_float(str, "digital_waterday", &wyd);
				if (digits < 1)
				    return FALSE;
				str += digits;

				/*
			 	 *   Convert wateryear decima days into date-time.
			 	 */
				wday = (int) floor(wyd);
				total_secs = DAY_TO_SEC(wyd - wday);

				if ((wday < 1) || (wday > 366)) {
				    usrerr("Invalid waterday: %d", wday);
				    return FALSE;
				}

				/*
				 * add 1 to wday for 0-start day adjustment
				 */
				wday++;

				have_waterday = TRUE;
				have_wyday = TRUE;
				break;

			    case '1' :
				if (! fcode_compatible(FCODE_W1))
				    return FALSE;

				digits = read_float(str, "digital_waterday", &wyd);
				if (digits < 1)
				    return FALSE;
				str += digits;

				/*
			 	 *  Convert wateryear decimal days into date-time.
			 	 */
				wday = (int) floor(wyd);
				total_secs = DAY_TO_SEC(wyd - wday);

				if ((wday < 1) || (wday > 366)) {
				    usrerr("Invalid waterday: %d", wday);
				    return FALSE;
				}

				have_waterday = TRUE;
				have_wyday = TRUE;
				break;

			    default :
				usrerr("Unknown format code: %W%c", *format);
				return FALSE;
			} /* switch for %W format codes */
			break;

		    case 'w' :
			format++;
			switch (*format) {
			    case EOS :
				usrerr("Incomplete format code at end of format string");
				return FALSE;
	
			    case 'd' :
				if (! fcode_compatible(FCODE_wd))
				    return FALSE;
				digits = read_int(str, 1, 3, "waterday", &wday);
				if (digits < 1)
				    return FALSE;
				str += digits;
				if ((wday < 1) || (wday > 366)) {
				    usrerr("Invalid waterday: %d", wday);
				    return FALSE;
				}
				have_waterday = TRUE;
				break;
	
			    case 'y' :
				if (! fcode_compatible(FCODE_wy))
				    return FALSE;
				digits = read_int(str, 2, 2, "wateryear", &wyear);
				if (digits < 2)
				    return FALSE;
				str += digits;
				wyear += century_wy;
				break;
	
			    case 'Y' :
				if (! fcode_compatible(FCODE_wY))
				    return FALSE;
				digits = read_int(str, 1, 4, "wateryear", &wyear);
				if (digits < 1)
				    return FALSE;
				str += digits;
				break;

			    default :
				usrerr("Unknown format code: %w%c", *format);
				return FALSE;
			} /* switch for %w format codes */
			break;

		    case 'H' :
			if (! fcode_compatible(FCODE_H))
			    return FALSE;
			digits = read_int(str, 1, 2, "hours", &hours);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if (hours > 23) {
			    usrerr("Invalid hours: %d", hours);
			    return FALSE;
			}
			break;

		    case 'M' :
			if (! fcode_compatible(FCODE_M))
			    return FALSE;
			digits = read_int(str, 1, 2, "minutes", &minutes);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if (minutes > 59) {
			    usrerr("Invalid minutes: %d", minutes);
			    return FALSE;
			}
			break;

		    case 'S' :
			if (! fcode_compatible(FCODE_S))
			    return FALSE;
			digits = read_int(str, 1, 2, "seconds", &seconds);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if (seconds > 59) {
			    usrerr("Invalid seconds: %d", seconds);
			    return FALSE;
			}
			break;

		    case 'h' :
			if (! fcode_compatible(FCODE_h))
			    return FALSE;
			digits = read_int(str, 1, 2, "hours", &hours);
			if (digits < 1)
			    return FALSE;
			str += digits;
			if ((hours < 1) || (hours > 12)) {
			    usrerr("Invalid hours for 12-hour clock: %d",
				   hours);
			    return FALSE;
			}
			have_12hr_time = TRUE;
			break;

		    case 'a' :
			if (! fcode_compatible(FCODE_a))
			    return FALSE;
			if (*str == EOS) {
			    usrerr("Found end-of-string instead of \"am\" or \"pm\"");
			    return FALSE;
			}
			if ( ((*str == 'a') || (*str == 'p')) &&
				(*(str+1) == 'm') ) {
			    am_pm = *str;
			    str += 2;
			    have_12hr_time = TRUE;
			} else {
			    usrerr("Found \"%.2s\" instead of \"am\" or \"pm\"",
				   str);
			    return FALSE;
			}
			break;

		    case 'A' :
			if (! fcode_compatible(FCODE_A))
			    return FALSE;
			if (*str == EOS) {
			    usrerr("Found end-of-string instead of \"AM\" or \"PM\"");
			    return FALSE;
			}
			if ( ((*str == 'A') || (*str == 'P')) &&
				(*(str+1) == 'M') ) {
			    am_pm = *str;
			    str += 2;
			    have_12hr_time = TRUE;
			} else {
			    usrerr("Found \"%.2s\" instead of \"AM\" or \"PM\"",
				   str);
			    return FALSE;
			}
			break;

		    case '%' :
			if (*str == EOS) {
			    usrerr("Found end-of-string instead of '%%'");
			    return FALSE;
			}
			if (*str != '%') {
			    usrerr("Invalid character: '%c', expecting this: '%%'",
				   *str);
			    return FALSE;
			}
			str++;
			break;

		    case '?' :  /* any character */
			if (*str == EOS) {
			    usrerr("Found end-of-string instead of a character");
			    return FALSE;
			}
			str++;
			break;

		    case '+' :  /* 1 or more whitespace characters */
			if (*str == EOS) {
			    usrerr("Found end-of-string instead of whitespace");
			    return FALSE;
			}
			if (! isspace(*str)) {
			    usrerr("Invalid character: '%c', expecting whitespace",
				   *str);
			    return FALSE;
			}
			str++;
			/*
			 * FALL THROUGH TO NEXT CASE: 0 or more whitespace
			 */

		    case '*' : /* 0 or more whitespace characters */
			while (isspace(*str))
			    str++;
			break;

		    default :
			usrerr("Unknown format code: %%c", *format);
			return FALSE;
		}  /* switch */

		/*
		 *  Advance past last character of format code
		 */
		format++;
	    }
	}  /* while */

	if (*str != EOS) {
	    usrerr("Expecting end-of-string but found character: '%c'", *str);
	    return FALSE;
	}

	/*
	 *  Perform any additional checks, and convert date to year, month, day
	 *  and time to 24-hour clock.
	 */
	if (have_month_day) {
	    if (day > numdays(year, month)) {
		usrerr("Invalid day: %d", day);
		return FALSE;
	    }
	}

	if (have_yearday) {
	    if (!leapyear(year) && (yday == 366)) {
		usrerr("Invalid leapyear yearday: %d: %d", yday, year);
		return FALSE;
	    }
	    yday2mday(year, yday, &month, &day);
	    if (have_jday) {
		sec2hms(total_secs, &hours, &minutes, &seconds);
	    }
	}

	if (have_waterday) {
	    if (!leapyear(wyear) && (wday == 366)) {
		usrerr("Invalid waterday: %d", wday);
		return FALSE;
	    }
	    wday2mday(wyear, wday, &year, &month, &day);
	    if (have_wyday) {
		sec2hms(total_secs, &hours, &minutes, &seconds);
	    }
	}

	if (have_12hr_time) {
	    hours = hours % 12;
	    if ((am_pm == 'p') || (am_pm == 'P'))
		hours += 12;
	}

	/*
	 *  Copy date and time.
	 */
	dt->year = year;
	dt->month = month;
	dt->day = day;
	dt->hour = hours;
	dt->min = minutes;
	dt->sec = seconds;

	return TRUE;
}
