/*
** NAME
** 	trigbuf -- allocate trigonometric look-up tables for ghorizon
** 
** SYNOPSIS
**	void trigbuf (nlines, nsamps, got_solar, maskout, max_pts_curv, fds,
**		      acostbl, asintbl, tzentbl,
**		      coslon, sinlon, cosdist, sindist)
**	int nlines, nsamps;
**	bool_t got_solar, maskout;
**	int max_pts_curv;
**	int fds;
**	float **acostbl, **asintbl, **tzentbl;
**	double **cosdist, **sindist;
** 
** DESCRIPTION
**	trigbuf allocates the look-up tables for the sines and cosines of
**	all possible quantized values of solar azimuth, the tangents of all
**	possible zeniths, the sines and cosines of all latitudes and longitudes
**	and the sined and cosines of all increments of distances for the great
**	circle path calculations.
** 
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
trigbuf(
	int		nlines,		/* # lines in input image	 */
	int		nsamps,		/* # samples in input image	 */
	bool_t		got_solar,	/* flag for solar image given	 */
	bool_t		maskout,	/* flag for mask image output	 */
	int		max_pts_curv,	/* max #pts based on Earth curv	 */
	int             fds,		/* solar file desc		 */
	float	      **acostbl,	/* -> table of cosine azimuth	 */
	float	      **asintbl,	/* -> table of sine azimuth	 */
	float	      **tzentbl,	/* -> table of tan zenith 	 */
	double	      **coslon,		/* -> table of cos(longitudes)	 */
	double	      **sinlon,		/* -> table of sin(longitudes)	 */
	double	      **cosdist,	/* -> table of cos(dist)	 */
	double	      **sindist)	/* -> table of sin(dist)	 */
{
	int	       *maplen;		/* length of solar FP maps	 */


   /* buffers to hold sines and cosines of solar azimuth angles */
   /* buffer to hold zenith angles in radians */

	if (maskout) {
		maplen = fpmaplen (fds);
		*acostbl = (float *) ecalloc (maplen[1], sizeof(float));
		if (*acostbl == NULL)
			error ("can't allocate acostbl");
		*asintbl = (float *) ecalloc (maplen[1], sizeof(float));
		if (*asintbl == NULL)
			error ("can't allocate asintbl");
		*tzentbl = (float *) ecalloc (maplen[0], sizeof(float));
		if (*tzentbl == NULL)
			error ("can't allocate tzentbl");
	} else if (got_solar) {
		maplen = fpmaplen (fds);
		*acostbl = (float *) ecalloc (maplen[0], sizeof(float));
		if (*acostbl == NULL)
			error ("can't allocate acostbl");
		*asintbl = (float *) ecalloc (maplen[0], sizeof(float));
		if (*asintbl == NULL)
			error ("can't allocate asintbl");
	}

   /* buffers to hold sines and cosines of all longitudes */

	*coslon = (double *) ecalloc (nsamps, sizeof(double));
	if (*coslon == NULL)
		error ("can't allocate coslon table");
	*sinlon = (double *) ecalloc (nsamps, sizeof(double));
	if (*sinlon == NULL)
		error ("can't allocate sinlon table");

   /* buffers to hold sines and cosines of distances for GCP calculations */

	*cosdist = (double *) ecalloc (max_pts_curv, sizeof(double));
	if (*cosdist == NULL)
		error ("can't allocate cosdist table");
	*sindist = (double *) ecalloc (max_pts_curv, sizeof(double));
	if (*sindist == NULL)
		error ("can't allocate sindist table");
}
