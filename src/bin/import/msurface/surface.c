/*
** NAME
** 	surface -- interpolate and write output image
** 
** SYNOPSIS
**	void
**	surface (fdm, fdo, npoints, points, nclosest,
**		 bline, bsamp, dline, dsamp)
**	int fdm, fdo;
**	int npoints;
**	struct point *points;
**	int nclosest;
**	double bline, bsamp, dline, dsamp;
** 
** DESCRIPTION
** 	surface interpolates for each output pixel using nearest neighbor
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
surface (
	int		fdi,		/* input image file descriptor	 */
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output image file descriptor	 */
	int		npoints,	/* # input data points		 */
	int		nclosest,	/* # neighbors desired           */
	double		bline,		/* line-coord of UL image corner */
	double		bsamp,		/* samp-coord of UL image corner */
	double		dline,		/* line spacing			 */
	double		dsamp,		/* sample spacing		 */
	int		inbands)	/* # bands in input file         */
{
	int		i, j;
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	double		northing;	/* northing of output pixel	 */
	double		easting;	/* easting of output pixel	 */
	pixel_t	      **ibuf;		/* input near-neib buffer	 */
	pixel_t	      **ibufp;		/* -> near-neib buffer		 */
	pixel_t	       *mbuf;		/* mask line buffer		 */
	pixel_t	       *mbufp;		/* -> in mask buffer		 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */

   /* allocate input and mask buffers */

	ibuf = (pixel_t **) allocnd (sizeof(pixel_t), 2, nsamps, inbands);
	if (ibuf == NULL) {
		error ("can't allocate input near-neib buffer");
	}

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

   /* allocate array of nearest neighbor indexes */

 	close_id = (pixel_t *) ecalloc (nclosest, sizeof(pixel_t));
	if (close_id == NULL) {
		error ("can't allocate close_id array");
	}

   /* loop interpolating and writing complete lines of data */

	northing = bline;
	easting = bsamp;

	for (line = 0; line < nlines; line++) {

if(line % 50 == 0) fprintf(stderr,"  %d lines to go\n",nlines - line);

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
			    error ("error reading mask image, line %d", line);
			}
		}

		/* read closest neighbors from input file */

		if (pvread (fdi, ibuf[0], nsamps) != nsamps) {
		    error ("error reading input image, line %d", line);
		}

		ibufp = ibuf;  /* rewind buffer pointers */
		obufp = obuf;
		mbufp = mbuf;
	
		for (samp = 0; samp < nsamps; samp++) {

		    if (fdm == ERROR || *mbufp++) {  /* if not masked */

		        /* read this cell's neibs into the array close_id */

		        for (i = 0, j = 0; j < nclosest && i < inbands; i++) {

			    if(points[ibufp[samp][i]].z != naflag) { 
				close_id[j++] = ibufp[samp][i];

			    }
			}

			/* if any close_id items aren't set, set to naflag */

			while (j < nclosest) {
			    close_id[j++] = -1;
/**
fprintf(stderr,"SETTING TO -1 ...\n");
dump(nclosest, 175, 174);
**/
			}
				

			/* get near-neib distances */
			
			get_distance (easting, northing, npoints, nclosest);

			/* interpolate */

			*obufp++ = interp (nclosest);

		    } else {	/* masked point */

				*obufp++ = NODATA;
			/***&&&
				ibufp += nclosest;
			***/

		    }

/*
if(line == 14 && samp == 64) dump(nclosest, 2, 1);
*/

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

void
dump(
	int	n,
	int	line,
	int	when)
{

  int i, j;

if(line >= when) {

    for(i=0; i<n; i++) {

	/***
	fprintf(stderr," %d", close_id[i]);
	***/
	if(close_id[i] > 0) {
	  fprintf(stderr," %d %.2f (%lf) \n", close_id[i], 
		neighbors[i].value, neighbors[i].dist2);
	}
    }
}

}
