/*
** NAME
** 	deltaz
** 
** SYNOPSIS
**	void
**	deltaz (fdm, fdo, npoints, points, nclosest,
**		 bline, bsamp, dline, dsamp)
**	int fdm, fdo;
**	int npoints;
**	struct point *points;
**	int nclosest;
**	double bline, bsamp, dline, dsamp;
** 
** DESCRIPTION
** 	deltaz interpolates for each output pixel using nearest neighbor
**	inverse-distance squared interpolation and writes the output
**	IPW image file.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
deltaz (
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output image file descriptor	 */
	int		npoints,	/* # input data points		 */
	struct point   *points,		/* linked list of input points	 */
	int		nclosest,	/* # neighbors in interp	 */
	double		bline,		/* line-coord of UL image corner */
	double		bsamp,		/* samp-coord of UL image corner */
	double		dline,		/* line spacing			 */
	double		dsamp)		/* sample spacing		 */
{
	int		nlines;		/* # lines in output image	 */
	int		nsamps;		/* # samples in output image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	double		northing;	/* northing of output pixel	 */
	double		easting;	/* easting of output pixel	 */
	fpixel_t       *mbuf;		/* mask line buffer		 */
	fpixel_t       *mbufp;		/* -> in mask buffer		 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	fpixel_t	dem_z; 		/* DEM elevation		 */
	struct neighbor *neighbors;	/* array of neighbors		 */


	nlines = hnlines (fdo);
	nsamps = hnsamps (fdo);


   /* allocate neighbors array */

	neighbors = (struct neighbor *) ecalloc (nclosest,
						  sizeof(struct neighbor));
	if (neighbors == NULL) {
		error ("error allocating neighbors array");
	}

   /* allocate DEM/mask buffer */

	if (fdm != ERROR) {
		mbuf = (fpixel_t *) ecalloc (nsamps * nbands, sizeof(fpixel_t));
		if (mbuf == NULL) {
			error ("can't allocate DEM/mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	northing = bline;
	easting = bsamp;

   /* loop interpolating and writing complete lines of data */

	for (line = 0; line < nlines; line++) {

		/* read line of mask image */

		if (fdm != ERROR) {
			if (fpvread (fdm, mbuf, nsamps) != nsamps) {
			  error ("error reading DEM/mask image, line %d", line);
			}
		}

		obufp = obuf;
		mbufp = mbuf;

		for (samp = 0; samp < nsamps; samp++) {

			/* read elevation value */

			dem_z = *mbufp++;			

			if (nbands == 1 || *mbufp++) {

				/* find closest neighbors */

				find_neighbors (northing, easting, npoints, 
					points, nclosest, neighbors);

				/* interpolate */

				*obufp++ = interp (nclosest, neighbors, dem_z);

			} else {	/* masked point */
				*obufp++ = NODATA;
			}

			easting += dsamp;
		}
	
		/* write output line */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("error writing output image, line %d", line);
		}

		northing += dline;
		easting = bsamp;
	}
}
