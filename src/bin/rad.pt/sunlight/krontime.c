#include <time.h>
#include "ipw.h"

#include "pgm.h"

/*
** NAME
**	krontime -- times and weights for Kronrod quadrature
**
** SYNOPSIS
**	#include "pgm.h"
**
**	struct qt **
**	krontime(
**		int          nkpts,	|* # Kronrod quadruature pts	 *|
**		datetime_t  *t1,	|* time at beginning		 *|
**		datetime_t  *t2,	|* time at end			 *|
**		double       lat,	|* latitude (radians)		 *|
**		double       lon)	|* longitude (radians)		 *|
**
** DESCRIPTION
**	Creates time structures for Kronrod quadrature between t1 and
**	t2.  nkpts is the number of Kronrod points, currently either 15
**      or 21.  lat and lon are in radians.  Returned is an array of
**	pointers to time structures.
**
** RESTRICTIONS
**	nkpts must be either 15 or 21.
**
** RETURN VALUE
**	NULL is returned on EOF or error.
**
*/

struct qt **
krontime(
	int             nkpts,		/* # Kronrod quadruature pts	 */
	datetime_t     *t1,		/* time at beginning		 */
	datetime_t     *t2,		/* time at end			 */
	double          lat,		/* latitude (radians)		 */
	double          lon)		/* longitude (radians)		 */
{
	struct qt     **qp;		/* -> return value		 */
	double		diff_secs;	/* diff between t1 and t2 (secs) */
	int		secs;		/* diff between t1 and t2 (secs) */
	int             dummy;		/* dummy argument for dt_diff    */
	double          tod2;		/* time of day (seconds) for t2	 */
	double         *xgk;		/* -> abscissae			 */
	double         *wgk;		/* -> weights			 */
	int             j;

 /*
  * array of pointers to results
  */
 /* NOSTRICT */
	qp = (struct qt **) ecalloc(nkpts, sizeof(struct qt *));
	if (qp == NULL) {
		return (NULL);
	}
	for (j = 0; j < nkpts; ++j) {
 /* NOSTRICT */
		qp[j] = (struct qt *) ecalloc(nkpts, sizeof(struct qt));
		if (qp[j] == NULL) {
			return (NULL);
		}
	}

 /*
  *  # of seconds between t1 and t2
  */
	dt_diff(t1, t2, &dummy, &secs);
	diff_secs = secs;

 /*
  * Kronrod abscissae and weights
  */
 /* NOSTRICT */
	xgk = (double *) ecalloc(nkpts, sizeof(double));
	if (xgk == NULL)
		return (NULL);
 /* NOSTRICT */
	wgk = (double *) ecalloc(nkpts, sizeof(double));
	if (wgk == NULL)
		return (NULL);
	switch (nkpts) {
	case 15:
		vqk15((double) SEC_DAY, 0, diff_secs, xgk, wgk);
		break;
	case 21:
		vqk21((double) SEC_DAY, 0, diff_secs, xgk, wgk);
		break;
	default:
		usrerr("nkpts = %d");
		return (NULL);
	}
 /*
  * transfer the weights to the output structures, compute time
  * structures, solar zenith angles and azimuths
  */
	for (j = 0; j < nkpts; ++j) {
		(qp[j])->q_x = xgk[j];
		(qp[j])->q_wt = wgk[j];

		/*
		 *  Add time difference in xgk array to start time
		 */
		(qp[j])->q_tp = make_dt(t1->year, t1->month, t1->day,
					t1->hour, t1->min, t1->sec);
		add_to_dt((qp[j])->q_tp, 0, (int) (xgk[j] + 0.5));

#if 0
		if (ephemeris((qp[j])->q_tp, &rv, &declin, &omega) == ERROR)
			return (NULL);
		switch (sunpath(lat, lon, declin, omega,
			      &((qp[j])->q_cos), &((qp[j])->q_azm))) {
		case 1:		/* sun below horizon */
			(qp[j])->q_cos = 0;
			break;
		case ERROR:
			return (NULL);
		default:
			;
		}
#endif
		switch (sunangle(lat, lon, (qp[j])->q_tp, &((qp[j])->q_cos),
							  &((qp[j])->q_azm),
							  NULL)) {
		    case 1:	/* sun below horizon */
			(qp[j])->q_cos = 0;
			break;
		    case ERROR:
			return (NULL);
		    default:
			;
		}
	}  /* for */

	return (qp);
}
