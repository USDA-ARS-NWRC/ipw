/*
** NAME
** 	ggradient -- orchestrate computation of global-scale gradient
** 
** SYNOPSIS
**	ggradient (fdi, fdm, fdo, slope, aspect, blat, blon, dlat, dlon);
** 	int fdi, fdm, fdo;
**	bool_t slope, aspect;
**	double blat, blon, dlat, dlon;
** 
** DESCRIPTION
**
**	 orchestrate computation of gradient over global scale image
**
**		In trying to understand this, remember that the definition of
**		x- and y-coordinates is the standard cartographic one:
**		y increases from west to east and x increases from north to
**		south.
**
**		read input data, allocate buffers,
**		call routine to compute gradient for each pixel
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
ggradient(
	int             fdi,		/* input image file descriptor	 */
	int             fdm,		/* mask image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	bool_t		slope,		/* flag to compute slope	 */
	bool_t		aspect,		/* flag to compute aspect	 */
	double		blat,		/* Northernmost lat (deg)	 */
	double		blon,		/* Easternmost long (deg)	 */
	double		dlat,		/* latitude spacing (deg)	 */
	double		dlon)		/* longitude spacing (deg)	 */
{
	fpixel_t       *a;		/* vector of aspect (rads)	 */
	fpixel_t      **ibuf;		/* input buffers		 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *s;		/* vector of sinS		 */
	fpixel_t       *temp;		/* for exchange			 */
	pixel_t	       *mbuf;		/* mask buffer			 */
	int             j;		/* counter			 */
	int		line;		/* line loop counter		 */
	int             ngot;		/* # samples read		 */
	int             nlines;		/* # lines			 */
	int             nsamps;		/* # samples / line		 */
	double		lat_dist;	/* distance between lat pixels	 */
	double	       *lon_dist;	/* dist between lon (per lat)	 */
	double	       *lonp;		/* -> in lon_dist lookup table	 */
	double		lat_r;		/* latitude (radians)		 */
	double		lon_r;		/* longitude (radians)		 */
	double		dlat_r;		/* latitude spacing (radians)	 */
	double		dlon_r;		/* longitude spacing (radians)	 */
	double		dist;		/* calculated distance (meters)	 */
	double		azm;		/* azimuth			 */
	double		deg2rad;	/* conversion: degrees to radians*/


	deg2rad = M_PI / 180.0;

	nsamps = hnsamps(fdi);
	nlines = hnlines(fdi);

   /* allocate lookup table for longitude spacing (indexed on row) */

	lon_dist = (double *) ecalloc (nlines, sizeof (double));
	if (lon_dist == NULL) {
		error("Can't allocate longitude distance lookup table");
	}

   /* fill lookup table of cell spacing between samples on a line */
   /* Note: great circle distance between cells along the same line */
   /* since they have the same latitude */

	lat_r = blat * deg2rad;
	lon_r = blon * deg2rad;
	dlat_r = dlat * deg2rad;
	dlon_r = dlon * deg2rad;

	lonp = lon_dist;

	for (line = 0; line < nlines; line++) {

		gcp_dist (lat_r, lon_r, lat_r, lon_r+dlon_r, &dist, &azm);
		*lonp++ = dist;
		lat_r += dlat_r;
	}

   /* calculate spacing between cells one line apart, same sample index */

	gcp_dist (blat, blon, blat+dlat_r, blon, &lat_dist, &azm);


   /* Allocate 3 input buffers of length nsamps+2. We need the extra 2 */
   /* spaces on end to calculate finite differences at end */

	ibuf = (fpixel_t **) allocnd (sizeof(fpixel_t), 2, 3, nsamps + 2);
	if (ibuf == NULL) {
		error("Can't allocate input buffer");
	}

   /* allocate buffers to hold calculates slope and aspect */

	s = NULL;
	a = NULL;
	if (slope) {
		s = (fpixel_t *)ecalloc (nsamps, sizeof(fpixel_t));
	}
	if (aspect) {
		a = (fpixel_t *)ecalloc (nsamps, sizeof(fpixel_t));
	}
	if ((slope && s == NULL) || (aspect && a == NULL)) {
		error("Can't allocate s and/or a vectors");
	}


   /*
    * allocate output buffer (we only need a special buffer if we're
    * going to merge slope and aspect later)
    */

	if (slope && aspect) {
		if ((obuf = (fpixel_t *)
		     ecalloc(2 * nsamps, sizeof(fpixel_t))) == NULL) {
			error("Can't allocate output buffer");
		}
	} else if (slope) {
		obuf = s;
	} else {
		obuf = a;
	}

   /* allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("Can't allocate mask buffer");
		}
	} else {
		mbuf = NULL;
	}


   /* Because we allow an extra pixel at beginning and end of line, */
   /* increment input buffers to 'first' pixel in calculation */

	for (j = 0; j < 3; j++) {
		(ibuf[j])++;
	}


   /* initialization: read 1st line into buffer[1], 2nd line into */
   /* buffer[2] */

	for (j = 1; j < 3; j++) {
		if (fpvread (fdi, ibuf[j], nsamps) != nsamps)
			error ("Read error, line %d", j);
	}

   /* fill end pixels of line 1 and all of line 0 */

	fillstart (nsamps, ibuf[0], ibuf[1], ibuf[2]);

   /* loop: calculate and write gradients then shift buffers and read */
   /* next line */

	lonp = lon_dist;

	do {

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image");
			}
		}

		/* compute slope and aspect */

		gradu (nsamps, lat_dist, lonp, dlat, dlon, slope, aspect,
		       ibuf, s, a, fdm, mbuf, obuf);

		/* write output line */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("Can't write line");
		}

		/* switch buffers to prepare for next read */

		temp = ibuf[0];
		ibuf[0] = ibuf[1];
		ibuf[1] = ibuf[2];
		ibuf[2] = temp;
		lonp++;


   /*
    * read next line
    */

	} while ((ngot = fpvread(fdi, ibuf[2], nsamps)) > 0);

	if (ngot < 0) {
		error("Image read error");
	}

   /* read last mask image line */

	if (fdm != ERROR) {
		if (pvread (fdm, mbuf, nsamps) != nsamps) {
			error ("error reading mask image");
		}
	}

   /* still have to do the last line */

	fillstart (nsamps, ibuf[2], ibuf[0], ibuf[1]);
	gradu (nsamps, lat_dist, lonp, dlat, dlon, slope, aspect,
	       ibuf, s, a, fdm, mbuf, obuf);

   /* write last line */

	if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
		error ("Can't write last line");
	}

   /*
    * clean up
    */

	SAFE_FREE(lon_dist);
	SAFE_FREE(ibuf);

	if (slope) {
    		SAFE_FREE(s);
	}
	if (aspect) {
		SAFE_FREE(a);
	}
	if (slope && aspect) {
		SAFE_FREE(obuf);
	} 
	if (fdm != ERROR) {
		SAFE_FREE(mbuf);
	}
}
