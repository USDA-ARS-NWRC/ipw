/*
** NAME
** 	horizon -- calculate cosine horizon angle and/or test if sun below
**		   horizon for global image (ghorizon)
** 
** SYNOPSIS
**	void
**	horizon (line, samp, elev, nlines, nsamps, max_pts, slon,
**		 blat, blon, dlat, dlon, tanzen, maskout,
**		 step_size, emax, hcos, maskv);
**	int line, samp;
**	fpixel_t **elev;
**	int nlines, nsamps;
**	int max_pts;
**	double slon;
**	double blat, blon;
**	double dlat, dlon;
**	float tanzen;
**	float emax;
**	double step_size;
**	float *hcos;
**	int *maskv;
** 
** DESCRIPTION
** 	horizon computes the horizon angle for the elevation
**	point at pixel (line, samp) in the direction of the solar azimuth.
**	It returns the cosine of the horizon angle or, if maskout is set, it
**	returns the "sun-below-horizon" mask value: one (1) if the horizon
**	angle is less than the given tangent of the solar zenith angle and
**	0 otherwise.
**
**	Algorithm:
**		A great circle path is calculated in the direction of the
**	solar azimuth.  Each step the great circle is advanced in distance
**	by the given step size and the new latitude and longitude are
**	returned.  The elevation for the new point is interpolated using
**	bilinear interpolation with the 4 surrounding elevation	grid cells.
**	The slope is then computed, keeping track of the maximum
**	slope (the horizon).  If maskout is set, the slope is compared with
**	the tangent of the solar zenith angle, and the mask value is set
**	appropriately.
** 
** GLOBALS ACCESSED
**	globals accessed from pgm.h:
**		sinlat, coslat, sinlon, coslon, sinazm, cosazm,
**		sindist, cosdist
**
*/

#include <math.h>
#include "ipw.h"
#include "pgm.h"

/* slope function */

#define SLOPE(zi,zj,distance)  ( ((zj) <= (zi)) ? 0 : ((zj)-(zi)) / (distance) )

void
horizon(
	int		line,		/* line # for current elev pixel */
	int		samp,		/* samp # for current elev pixel */
	fpixel_t      **elev,		/* elevation buffer		 */
	int		nlines,		/* # lines in elev buffer	 */
	int		nsamps,		/* # samps in elev buffer	 */
	int		max_pts,	/* max #pts to search for horizon */
	double		slon,		/* start longitude (radians)	 */
	double		blat,		/* lat of image UL corner (rads) */
	double		blon,		/* lon of image UL corner (rads) */
	double		dlat,		/* lat spacing (radians)	 */
	double		dlon,		/* lon spacing (radians)	 */
	float		tanzen,		/* tangent solar zenith angle 	 */
	bool_t		maskout,	/* flag for mask image output	 */
	double		step_size,	/* GCP step size (meters)	 */
	float		emax,		/* maximum elevation in file	 */
	float	       *hcos,		/* return cos horizon angle	 */
	int	       *maskv )		/* return mask value		 */
{
	int		j;		/* loop counter			 */
	int		line1;		/* line index for elev interp	 */
	int		samp1;		/* sample index for elev interp	 */
	int		line2;		/* line index for elev interp	 */
	int		samp2;		/* sample index for elev interp	 */
	int		maxsamp;	/* max sample index (nsamps-1)	 */
	int		maxline;	/* max line index (nlines-1)	 */
	fpixel_t	elev_i;		/* elevation at point i		 */
	fpixel_t	elev_j;		/* elevation at point j		 */
	float		slope;		/* slope of current point	 */
	float		max_slope;	/* maximum slope		 */
	float		max_elev;	/* max elevation difference	 */
	float		max_dist;	/* max dist to beat max_slope	 */
	float		distance;	/* distance to current point	 */
	float		dline;		/* decimal line index		 */
	float		dsamp;		/* decimal sample index		 */
	double		lat;		/* latitude of new gcp point	 */
	double		lon;		/* longitude of new gcp point	 */
	double		lon1;		/* lon for elev interpolation	 */
	double		lon2;		/* lon for elev interpolation	 */
	double		lat1;		/* lat for elev interpolation	 */
	double		lat2;		/* lat for elev interpolation	 */
	double		alpha;		/* slope for elev interpolation	 */
	double		beta;		/* slope for elev interpolation	 */
	double		alpha_beta;	/* alpha * beta			 */
	double		elev_h;		/* elev at pt with max slope	 */
	double		dist_h;		/* distance to pt with max slope */
	double		elev_diff;	/* elevation diff at horizon	 */
	double		tempd;


   /* initialize */

	distance = 0.0;
	elev_i = elev[line][samp];
	max_slope = 0.0;
	max_elev = emax - elev_i;
	max_dist = max_pts * step_size;
	elev_h = elev_i;
	dist_h = distance;
	maxsamp = nsamps - 1;
	maxline = nlines - 1;

   /* initialize gcp function */

	init_gcp (slon, sinlat, coslat, sinlon[samp], coslon[samp],
		  sinazm, cosazm);

   /* loop computing slopes and comparing them with solar zenith */

	for (j = 0; j < max_pts; j++) {

		/* advance GCP calculation by current step size */
		/* break loop if maximum distance to beat current max slope */
		/* exceeded */

		distance += step_size;
		if (distance > max_dist) {
			slope = 0.0;
			break;
		}
		gcp (sindist[j], cosdist[j], &lat, &lon);

		/* calculate line and sample index of new point */

		dline = (lat - blat) / dlat;
		dsamp = (lon - blon) / dlon;

		/* check if out of image */

		if (dsamp < 0 || dsamp >= maxsamp ||
		    dline < 0 || dline >= maxline) {

			slope = 0.0;
			break;
		}

		/* interpolate for elevation at point: */
		/* use bilinear interpolation of 4 surrounding points */

		line1 = dline;
		line2 = line1 + 1;
		lat1 = blat + line1 * dlat;
		lat2 = lat1 + dlat;
		samp1 = dsamp;
		samp2 = samp1 + 1;
		lon1 = blon + samp1 * dlon;
		lon2 = lon1 + dlon;

		alpha = (lon - lon1) / dlon;
		beta =  (lat - lat1) / dlat;
		alpha_beta = alpha * beta;

		elev_j = (1 - alpha - beta + alpha_beta) * elev[line1][samp1] +
			 (beta - alpha_beta) * elev[line2][samp1] +
			 (alpha - alpha_beta) * elev[line1][samp2] +
			 alpha_beta * elev[line2][samp2];

		/* compute slope and compare to tangent of zenith angle */

		slope = SLOPE (elev_i, elev_j, distance);

		/* for mask output, quit if sun below horizon */
		/* otherwise keep track of max slope (horizon) */
		/* calculate maximum distance to "beat" the max slope: */
		/* distance where max_slope = (emax - elev) / distance */

		if (maskout) {
			if (slope > tanzen) {
				*maskv = 0;
				return;
			}
		}

		if (slope > max_slope) {
			max_slope = slope;
			max_dist = max_elev / max_slope;
			elev_h = elev_j;
			dist_h = distance;
		}

	}


	if (maskout) {
		*maskv = 1;
	} else {
		if (max_slope <= 0) {
			*hcos = 0.0;
		} else {
			elev_diff = (elev_h - elev_i);
			tempd = hypot(elev_diff, dist_h);
			*hcos = (float) (elev_diff / tempd );
		}
	}

}
