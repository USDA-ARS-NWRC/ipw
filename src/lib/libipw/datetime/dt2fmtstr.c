/*
 Acceptable format codes:
   %% -- a single %
   %y   --  year within century (last 2 digits)
   %Y   --    complete year (4 digits)
   %m   --  month as 2 digits (single digits preceeded by 0)
   %^m  --    month as number (1 or 2 digits)
   %d   --  day as 2 digits (single digits preceeded by 0)
   %^d  --    day as number (1 or 2 digits)
   %w   --  waterday, padded
   %^w  --    waterday as number (1-3 digits)
   %wy  --  water year within century
   %wY  --    complete wateryear (4 digits)
   %j   --  yearday, padded number
   %^j  --    yearday as number
   %Jd  --  Julian day (decimal days; Oct 1, midnight = 0.0)
   %J1  --  Julian day (decimal days; Oct 1, midnight = 1.0;
                        hence "jd1" is equivalent to "jd"+1)
   %Wd  --  wateryear day (decimal days; Oct 1, midnight = 0.0)
   %W1  --  wateryear day (decimal days; Oct 1, midnight = 1.0;
                           hence "wyd1" is equivalent to "wyd"+1)

   %H   --  hour of day as 2 digits (single digits preceeded by 0)
   %^H  --    hour of day as number (1 or 2 digits: 0-23)
   %h   --    hour of day as 2 digits (12-hour clock)
   %^h  --      hour of day as number (12-hour clock)
   %M   --  minute of day as 2 digits
   %S   --  seconds of day as 2 digits
   %a   --  "am" or "pm"
   %A   --  "AM" or "PM"
  all other codes yield an error
*/

#include "ipw.h"

bool_t
dt2fmtstr(
	  datetime_t	* dt,
	  char          * format,
          char          * buffer)
{
	int		hr12;		/* hour of day as 1 to 12 */
	int		wday = 0;
	int		wyear;
	int		yrday;
	double		jd;
	double		wyd;

	assert(dt != NULL);
	assert(format != NULL);
	assert(buffer != NULL);

	while (*format != EOS) {
	    if (*format != '%') {
		*buffer++ = *format++;
	    } else {
		format++;
		switch (*format) {
		    case '%' :
			*buffer++ = '%';
			break;

		    case 'y' :
			sprintf(buffer, "%02d", (dt->year % 100));
			buffer += 2;
			break;

		    case 'Y' :
			sprintf(buffer, "%04d", dt->year);
			buffer += 4;
			break;

		    case 'm' :
			sprintf(buffer, "%02d", dt->month);
			buffer += 2;
			break;

		    case 'd' :
			sprintf(buffer, "%02d", dt->day);
			buffer += 2;
			break;

		    case 'H' :
			sprintf(buffer, "%02d", dt->hour);
			buffer += 2;
			break;

		    case 'h' :
			hr12 = dt->hour % 12;
			if (hr12 == 0) {
				hr12 = 12;
			}
			sprintf(buffer, "%02d", hr12);
			buffer += 2;
			break;

		    case 'M' :
			sprintf(buffer, "%02d", dt->min);
			buffer += 2;
			break;

		    case 'S' :
			sprintf(buffer, "%02d", dt->sec);
			buffer += 2;
			break;

		    case 'a' :
			sprintf(buffer, "%cm", (dt->hour < 12) ? 'a' : 'p');
			buffer += 2;
			break;

		    case 'A' :
			sprintf(buffer, "%cM", (dt->hour < 12) ? 'A' : 'P');
			buffer += 2;
			break;

		    case 'j' :
			sprintf(buffer, "%03d", yearday(dt->year, dt->month,
							dt->day));
			buffer += 3;
			break;

		    case 'w' :
			if (wday == 0) {
			    wday = waterday(dt->year, dt->month, dt->day,
					    &wyear);
			}
			switch (*(format+1)) {
			    case 'y' :
				sprintf(buffer, "%02d", wyear % 100);
				buffer += 2;
				format++;
				break;

			    case 'Y' :
				sprintf(buffer, "%04d", wyear);
				buffer += 4;
				format++;
				break;

			    default :
				sprintf(buffer, "%03d", wday);
				buffer += 3;
				break;
			}
			break;

		    case 'J':
			format++;
			switch (*format) {
			    case 'd' :
				/*
	 		 	 *  Convert year, month, day -> yearday
	 		 	 */
				yrday = yearday(dt->year, dt->month, dt->day);

				/*
				 *  add 1 to wday for 0-start day adjustment
				 */
	    			yrday--;

				/*
	 		 	 *  Add time as decimal day to yearday
	 		 	 */
				jd = yrday + SEC_TO_DAY(HMS_TO_SEC(dt->hour, dt->min, dt->sec));

				sprintf(buffer, "%010.6f", jd);
				buffer += 10;
				break;

			    case '1':
				/*
	 		 	 *  Convert year, month, day -> yearday
	 		 	 */
				yrday = yearday(dt->year, dt->month, dt->day);

				/*
	 		 	 *  Add time as decimal day to yearday
	 		 	 */
				jd = yrday + SEC_TO_DAY(HMS_TO_SEC(dt->hour, dt->min, dt->sec));

				sprintf(buffer, "%010.6f", jd);
				buffer += 10;
				break;
			}
			break;

		    case 'W':
			format++;
			switch (*format) {
			    case 'd' :
				/*
			 	 *  Convert year, month, day -> waterday
			 	 */
				wday = waterday(dt->year, dt->month, dt->day, &wyear);

				/*
				 *  add 1 to wday for 0-start day adjustment
				 */
				wday--;

				/*
			 	 *  Add time as decimal day to waterday
			 	 */
				wyd = wday + SEC_TO_DAY(HMS_TO_SEC(dt->hour, dt->min, dt->sec));

				sprintf(buffer, "%010.6f", wyd);
				buffer += 10;
				break;

			    case '1':
				/*
			 	 *  Convert year, month, day -> waterday
			 	 */
				wday = waterday(dt->year, dt->month, dt->day, &wyear);
	
				/*
			 	 *  Add time as decimal day to waterday
			 	 */
				wyd = wday + SEC_TO_DAY(HMS_TO_SEC(dt->hour, dt->min, dt->sec));
	
				sprintf(buffer, "%010.6f", wyd);
				buffer += 10;
				break;
			}
			break;

		    case '^' :
			format++;
			switch (*format) {
			    case 'm' :
				sprintf(buffer, "%d", dt->month);
				buffer += (dt->month < 10 ? 1 : 2);
				break;
	
			    case 'd' :
				sprintf(buffer, "%d", dt->day);
				buffer += (dt->day < 10 ? 1 : 2);
				break;
	
			    case 'H' :
				sprintf(buffer, "%d", dt->hour);
				buffer += (dt->hour < 10 ? 1 : 2);
				break;
	
			    case 'h' :
				hr12 = dt->hour % 12;
				if (hr12 == 0) {
					hr12 = 12;
				}
				sprintf(buffer, "%d", hr12);
				buffer += (hr12 < 10 ? 1 : 2);
				break;

			    case 'j' :
				yrday = yearday(dt->year, dt->month, dt->day);
				sprintf(buffer, "%d", yrday);
				buffer += (yrday < 10 ? 1 : yrday < 100 ? 2
									: 3);
				break;
	
			    case 'w' :
				if (wday == 0) {
				    wday = waterday(dt->year, dt->month,
						    dt->day, &wyear);
				}
				sprintf(buffer, "%d", wday);
				buffer += (wday < 10 ? 1 : wday < 100 ? 2 : 3);
				break;

			    default :
				usrerr("Unknown format code: %%^%c", *format);
				return FALSE;
				break;
			}
			break;

		    default :
			usrerr("Unknown format code: %%%c", *format);
			return FALSE;
			break;
		}  /* switch */
		format++;
	    }
        }  /* while */

	*buffer = EOS;
	return TRUE;
}
