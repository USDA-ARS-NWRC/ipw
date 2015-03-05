#include <time.h>
#include <math.h>

#include "ipw.h"
#include "pgm.h"
#include "solar.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_d = {
		'd', "date (year, month, day)",
		INT_OPTARGS, "i",
		REQUIRED, 3, 3
	};

	static OPTION_T opt_b = {
		'b', "latitude (deg, min, sec unless -r specified)",
		REAL_OPTARGS, "lat",
		REQUIRED, 1, 3
	};

	static OPTION_T opt_l = {
		'l', "longitude (deg, min, sec unless -r specified)",
		REAL_OPTARGS, "lon",
		REQUIRED, 1, 3
	};

	static OPTION_T opt_r = {
		'r', "lat/lon in radians"
	};

	static OPTION_T opt_z = {
		'z', "time zone, minutes west of Greenwich",
		INT_OPTARGS, "zone",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_y = {
		'y', "daylight savings flag"
	};

	static OPTION_T opt_a = {
		'a', "abbreviated output for shell scripts"
	};

	static OPTION_T opt_q = {
		'q', "# quadrature points",
		INT_OPTARGS, "npts",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_d,
		&opt_b,
		&opt_l,
		&opt_r,
		&opt_z,
		&opt_y,
		&opt_q,
		&opt_a,
		0
	};

	bool_t          isdst;		/* ? daylight savings		 */
	double          daylen;		/* total day length (Hrs)	 */
	double		srise;		/* decimal sunrise time (Hrs)	 */
	double		snoon;		/* decimal solar noon time (Hrs) */
	double		sset;		/* decimal sunset time (Hrs)	 */
	int		daylen_hrs;	/* day length (days component)   */
	int		daylen_secs;	/* day length (seconds component)*/
	double          latitude;	/* latitude (radians)	 	 */
	double          longitude;	/* longitude (radians)	 	 */
	int             day;		/* day (1-31)		 	 */
	int             jt;		/* time index			 */
	int             kt;		/* # Kronrod pts		 */
	int             month;		/* month (1-12)		 	 */
	int             year;		/* year (1902-2037)	 	 */
	int             zone;		/* minutes west of Greenwich	 */
	struct qt     **qtime;		/* quadrature times/wts		 */
	datetime_t     *tprise;		/* -> structure to print	 */
	datetime_t     *tprnt;		/* -> structure to print	 */
	datetime_t     *tpset;		/* -> structure to print	 */
	datetime_t     *trise;		/* -> time structure	 	 */
	datetime_t     *tset;		/* -> time structure	 	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * arguments
  */
	sunlight_opts(&opt_d, &opt_b, &opt_l, &opt_r, &opt_z, &opt_y,
	   &zone, &isdst, &latitude, &longitude, &year, &month, &day);
 /*
  * compute sunrise/set
  */
	trise = sunrise(latitude, longitude, year, month, day);
	if (trise == NULL) {
		error("sunrise");
	}
	tset = sunset(latitude, longitude, year, month, day);
	if (tset == NULL) {
		error("sunset");
	}
	dt_diff(trise, tset, &daylen_hrs, &daylen_secs);
	daylen = daylen_hrs + SEC_TO_HR(daylen_secs);

 /*
  * convert to local time?
  */
	if (zone != 0 && daylen != 24 && daylen != 0) {
		tprise = (datetime_t *) ecalloc(1, sizeof(datetime_t));
		*tprise = *trise;
		gmt2local(tprise, zone, isdst);

		tpset = (datetime_t *) ecalloc(1, sizeof(datetime_t));
		*tpset = *tset;
		gmt2local(tpset, zone, isdst);
	}
	else {
		tprise = trise;
		tpset = tset;
	}

 /*
  * Results
  */
	if (!got_opt(opt_a)) {
		printf("Time zone is %s; day length is %.2f hrs\n",
		       zone2str(zone, isdst), daylen);
		printf("Latitude/longitude (radians): %.10g %.10g\n",
		       latitude, longitude);
		printf("Sunrise: %s", dt2str(tprise));
		printf("Sunset:  %s", dt2str(tpset));
	} else if (!got_opt(opt_q)) {
		srise = (HMS_TO_SEC(tprise->hour, tprise->min, tprise->sec) / 3600.0);
		snoon = (daylen / 2.0) + srise;
		sset = (HMS_TO_SEC(tpset->hour, tpset->min, tpset->sec) / 3600.0);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n", daylen, srise, snoon, sset);
	}
 /*
  * quadrature times and weights
  */
	if (got_opt(opt_q)) {
 /*
  * only 15 or 21 pts supported
  */
		kt = int_arg(opt_q, 0);
		switch (kt) {
		case 15:
		case 21:
			break;
		default:
			error("-q: only 15 or 21 pts supported");
		}
 /*
  * Kronrod pts
  */
		assert((qtime = krontime(kt, trise, tset,
				       latitude, longitude)) != NULL);
 /*
  * print
  */
		if (!got_opt(opt_a)) {
			printf("\n%d%s%s\n",
			       kt,
			       "-pt Kronrod quadrature weights, ",
			       "sun angles, and times.");
		}
		for (jt = kt; --jt >= 0;) {
 /*
  * convert to local time?
  */
			if (zone != 0 && daylen != 24 && daylen != 0) {
				gmt2local((*qtime)->q_tp, zone, isdst);
			}
			printf("%g %g %g",
			       (*qtime)->q_wt,
			       (*qtime)->q_cos,
			       RAD_TO_DEG( (*qtime)->q_azm ) );
			if (!got_opt(opt_a))
				printf(" %s", dt2str((*qtime)->q_tp) );
			else
				putchar('\n');
			++qtime;
		}
	}
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
