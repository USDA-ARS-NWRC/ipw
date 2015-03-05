#ifndef PGM_H
#define PGM_H
 
#include "IPWmacros.h"

 /*
  * structure to hold quadrature time and weight, along with solar
  * cosine and azimuth
  */

struct qt {
	datetime_t    *q_tp;		/* -> date and time      */
	double          q_x;		/* abscissa		 */
	double          q_wt;		/* weight		 */
	double          q_cos;		/* cosine solar angle	 */
	double          q_azm;		/* solar azimuth	 */
};

 /*
  * functions used
  */
extern struct qt  ** krontime(int nkpts, datetime_t *t1, datetime_t *t2,
			      double lat, double lon);
extern void          sunlight_opts(OPTION_T *d, OPTION_T *b, OPTION_T *l,
				   OPTION_T *r, OPTION_T *z, OPTION_T *y,
				   int *zone, bool_t *isdst, double *lat,
				   double *lon, int *year, int *month, int *day);
extern datetime_t  * sunrise(double lat, double lon, int year, int month,
			     int day);
extern datetime_t  * sunset(double lat, double lon, int year, int month,
			    int day);
extern void          vqk15(double tlen, double a, double b, double *x,
			   double *w);
extern void          vqk21(double tlen, double a, double b, double *x,
			   double *w);
extern void          vqkagn(int nkpt, int ngpt, double tlen, double a, double b,
			    double *x, double *w, double *xgk, double *wgk);

 /*
  * constants
  */
#define	RISE	0
#define	SET	1
#define MIN_DEG	60
#define SEC_DAY	86400
#define SEC_DEG 3600
#define SEC_HR	3600
#define SEC_MIN	60
#define TOL	1.e-5

#endif
