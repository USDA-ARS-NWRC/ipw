/*
** NAME
** 	iinterp -- transform image by given X,Y pairs
** 
** SYNOPSIS
**	iinterp (fdi, fdo, ifp, npoints, clip)
** 	int fdi, fdo;
**	FILE *ifp;
**	int npoints;
**	bool_t clip;
** 
** DESCRIPTION
** 	iinterp reads the given IPW image file, and writes an image file
**	where each of the image pixels is transformed according to a linear
**	interpolation on the given X,Y floating point pairs.
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "pgm.h"

void
iinterp (
	int		fdi,		/* input image file descriptor	 */
	int		fdo,		/* output image file descriptor	 */
	FILE	       *ifp,		/* file -> for points file	 */
	int		npoints,	/* # X,Y point pairs		 */
	bool_t		clip)		/* flag to clip at given X-vals  */
{
 	int		ilut;		/* lut index                     */
	int		nlines;		/* #lines in input image         */
	int		nsamps;		/* #samps in input image         */
	int		nbits;		/* #bits in input image		 */
	fpixel_t       *xval;		/* X-values of points		 */
	fpixel_t       *yval;		/* Y-values of points		 */
	fpixel_t       *lut;		/* lookup table for transform	 */
	fpixel_t      **fmap;		/* -> floating point maps	 */
	fpixel_t       *map;		/* -> band 0 floating point map	 */
	fpixel_t	min =0.0;	/* min transformed value	 */
	fpixel_t	max =0.0;	/* maximum transformed value	 */
	float		m =0.0;		/* slope for linear interp	 */
	float		b =0.0;		/* y-intercept for linear interp */
	int		i;		/* loop counter			 */
	int	       *fmaplen;	/* length of F.P. maps		 */
	int		maplen;		/* length of band 0 map		 */


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);
	nbits = hnbits (fdi, 0);


   /* allocate storage for X,Y arrays */

	xval = (fpixel_t *) ecalloc (npoints, sizeof(fpixel_t));
	if (xval == NULL) {
		error ("can't allocate points array");
	}

	yval = (fpixel_t *) ecalloc (npoints, sizeof(fpixel_t));
	if (yval == NULL) {
		error ("can't allocate points array");
	}

   /* read in X, Y pairs */

	get_points (ifp, npoints, xval, yval);

   /* access input image floating point map */

	fmap = fpmap (fdi);
	map = fmap[0];
	fmaplen = fpmaplen (fdi);
	maplen = fmaplen[0];

   /* allocate transform lookup table */

	lut = (fpixel_t *) ecalloc (maplen, sizeof(fpixel_t));
	if (lut == NULL) {
		error ("can't allocate transform lookup table");
	}

   /* perform transformation - initialize lookup table */

   /* clip points less than xval[0] if requested */

	ilut = 0;
	if (clip) {
		while ((ilut < maplen) && (map[ilut] < xval[0])) {
			lut[ilut] = yval[0];
			ilut++;
		}
		min = max = yval[0];
	}

   /* interpolate using X,Y pairs */

	i = 1;
	while (i < npoints) {

		m = (yval[i] - yval[i-1]) / (xval[i] - xval[i-1]);
		b = yval[i] - xval[i] * m;

		while ((ilut < maplen) && (map[ilut] <= xval[i])) {
			lut[ilut] = m * map[ilut] + b;
			if (ilut > 0) {
				min = MIN (min, lut[ilut]);
				max = MAX (max, lut[ilut]);
			} else {
				min = max = lut[ilut];
			}
			ilut++;
		}
		i++;
	}

   /* extrapolate or clip points > xval[maplen-1] */

	while (ilut < maplen) {
		if (clip) {
			lut[ilut] = yval[npoints-1];
		} else {
			lut[ilut] = m * map[ilut] + b;
		}
		min = MIN (min, lut[ilut]);
		max = MAX (max, lut[ilut]);
		ilut++;
	}

   /* create a new LQ header and prepare for image output */

	newlqh (fdo, min, max);

   /* map the image through the lookup table */

	lutximg (fdi, nlines, nsamps, lut, fdo);

}
