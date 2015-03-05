#ifndef PGM_H
#define PGM_H

#define OBANDS	1

#define TWOPI		(2.0 * M_PI)
#define PIO2		M_PI_2
#define PIO180		(M_PI / 180.0)

#include <math.h>
#include "IPWmacros.h"
#include "lqh.h"

extern double  *coslon;		/* -> table of cos(longitudes)	 */
extern double  *sinlon;		/* -> table of sin(longitudes)	 */
extern double	coslat;		/* cos(latitude)		 */
extern double	sinlat;		/* sin(latitude)		 */
extern double	cosazm;		/* cos of azimuth		 */
extern double	sinazm;		/* sin of azimuth		 */
extern double  *cosdist;	/* -> table of cos(dist)	 */
extern double  *sindist;	/* -> table of sin(dist)	 */

extern double   azmf (double azd);
extern void     gcp (double sin_dist, double cos_dist, double *lat, double *lon);
extern void     init_gcp (double slon, double sinlat, double coslat,
			  double sinlon, double coslon, double sinazm,
			  double cosazm);
extern void     gcp_dist (double sinlat1, double coslat1, double lon1,
			  double sinlat2, double coslat2, double lon2,
			  double *distance, double *azm);
extern void     ghorizon (int fdi, int fds, int fdm, int fdo, bool_t got_solar,
			  double azm, bool_t maskout, bool_t zenith, float emax,
			  double blat, double blon, double dlat, double dlon);
extern void     headers (int fdi, int fds, int fdm, int fdo, bool_t got_solar,
			 double azm, bool_t maskout, bool_t zenith, double *blat,
			 double *blon, double *dlat, double *dlon);
extern void     horizon (int line, int samp, fpixel_t **elev, int nlines,
			 int nsamps, int max_pts, double slon, double blat,
			 double blon, double dlat, double dlon, float tanzen,
			 bool_t maskout, double step_size, float emax,
			 float *hcos, int *maskv);
extern void     iobuf (int nlines, int nsamps, bool_t got_solar, int s_nbands,
		       int fdm, fpixel_t ***ibuf, pixel_t **mbuf,
		       pixel_t **sbuf, fpixel_t **obuf);
extern LQH_T ** newlqh (int fdo);
extern void     trigbuf (int nlines, int nsamps, bool_t got_solar,
			 bool_t maskout, int max_pts_curv, int fds,
			 float **acostbl, float **asintbl, float **tzentbl,
			 double **coslon, double **sinlon, double **cosdist,
			 double **sindist);
extern void     trigtbl (int fds, bool_t got_solar, bool_t maskout,
			 float *asintbl, float *acostbl, float *tzentbl,
			 bool_t zenith, double *sinlon, double *coslon,
			 double blon, double dlon, int nsamps, double *sindist,
			 double *cosdist, int maxpts, double step_size);

#endif
