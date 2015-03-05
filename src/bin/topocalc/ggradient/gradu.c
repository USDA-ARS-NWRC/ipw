/*
** NAME
** 	gradu -- compute slopes and aspects for ggradient
** 
** SYNOPSIS
**	void
**	gradu (nsamps, lat_dist, lon_dist, dlat, dlon, dos, doa, ibuf,
**	       slope, aspect, fdm, mbuf, obuf)
**	int nsamps;
**	double lat_dist;
**	double *lon_dist;
**	double dlat, dlon;
**	bool_t dos;
**	bool_t doa;
**	fpixel_t **ibuf;
**	fpixel_t *slope;
**	fpixel_t *aspect;
**	int fdm;
**	pixel_t *mbuf;
**	fpixel_t *obuf;
** 
** DESCRIPTION
**	This is where the work is done.  Compute derivatives, slopes and
**	apsects, and shuffle.
**
*/

#include <math.h>
#include "ipw.h"
#include "pgm.h"

void
gradu(
	int             nsamps,		/* # samples in line		 */
	double		lat_dist,	/* grid cell line spacing (m)	 */
	double         *lon_dist,	/* grid cell sample spacing (m)	 */
	double		dlat,		/* grid cell line spacing (deg)	 */
	double		dlon,		/* grid cell sample spacing (deg)*/
	bool_t          dos,		/* ? compute slope		 */
	bool_t          doa,		/* ? compute aspect		 */
	fpixel_t      **ibuf,		/* input buffers		 */
	fpixel_t       *slope,		/* sinS				 */
	fpixel_t       *aspect,		/* aspect			 */
	int		fdm,		/* mask file descriptor		 */
	pixel_t	       *mbuf,		/* mask line buffer		 */
	fpixel_t       *obuf)		/* output buffer		 */
{
	int		samp;		/* sample loop counter		 */
	float		dx;		/* partial w.r.t. x		 */
	float		dy;		/* partial w.r.t. y		 */
	float		s_xfactor;	/* 1/denom of slope dz/dx	 */
	float		s_yfactor;	/* 1/denom of slope dz/dy	 */
	float		a_xfactor;	/* 1/denom of aspect dz/dx	 */
	float		a_yfactor;	/* 1/denom of aspect dz/dy	 */
	float		tanS;		/* tangent slope		 */
	pixel_t	       *mbufp;		/* -> mask line buffer		 */
	fpixel_t       *s;		/* -> in sinS			 */
	fpixel_t       *a;		/* -> in aspect			 */
	fpixel_t       *k0;		/* top line of kernel		 */
	fpixel_t       *k1;		/* middle line of kernel	 */
	fpixel_t       *k2;		/* last line of kernel		 */


   /* fill end pixels of line 2 */

	fillends (nsamps, ibuf[2]);

   /* for slope calculation use distance in meters for partial difference */

	s_xfactor = 1.0 / (lon_dist[0] + lon_dist[1]);
	s_yfactor = 1.0 / (2.0 * lat_dist);

   /* for aspect calculation use distance in degrees for partial difference */

	a_xfactor = 1.0 / (2.0 * dlon);
	a_yfactor = -1.0 / (2.0 * dlat);

	k0 = ibuf[0];
	k1 = ibuf[1];
	k2 = ibuf[2];
	s = slope;
	a = aspect;
	mbufp = mbuf;

   /* loop on samples computing slope and aspect */

	for (samp = 0; samp < nsamps; samp++) {

		/* compute slope:				*/
		/*	Note that tan S = sqrt(dx*dx + dy*dy)	*/
		/*	If (tanS == b)				*/
		/*		sinS = sqrt(b/(1+b));		*/

		if (dos) {
			if ((fdm == ERROR) || *mbufp) {
				dx = (*k2 - *k0) * s_xfactor;
				dy = ((k1[1]) - (k1[-1])) * s_yfactor;
				if (dx != 0 || dy != 0) {
					tanS = dx * dx + dy * dy;
					*s = sqrt (tanS / (1 + tanS));
					/*
					 * I don't know why this is so bad.
					 *
					if (*s > .9) {
						warn ("slope is very large.\n");
					}
					*/
					s++;
				} else {
					*s++ = 0;
				}
			} else {
				*s++ = 0;
			}
		}

		/* compute aspect:				*/

		if (doa) {
			if ((fdm == ERROR) || *mbufp) {
				dx = (*k2 - *k0) * a_xfactor;
				dy = ((k1[1]) - (k1[-1])) * a_yfactor;
				if (dx != 0 || dy != 0) {
					*a++ = atan2 ((double) -dy, (double) -dx);
				} else {
					*a++ = 0;
				}
			} else {
				*a++ = 0;
			}
		}

		k0++;
		k1++;
		k2++;
		mbufp++;
	}

   /* shuffle into output buffer if both slopes and aspects */

	if (dos && doa) {
		shuffle (nsamps, slope, aspect, obuf);
	}
}
