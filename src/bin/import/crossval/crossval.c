/*
**
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
crossval (
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
	int		i;
	int		nlines;		/* # lines in output image	 */
	int		nsamps;		/* # samples in output image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	double		northing;	/* northing of output pixel	 */
	double		easting;	/* easting of output pixel	 */
	pixel_t	       *mbuf;		/* mask line buffer		 */
	pixel_t	       *mbufp;		/* -> in mask buffer		 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	struct neighbor *neighbors;	/* array of neighbors		 */
	FILE		*outfile;	/* ascii output file    	 */
  	float 		 jackval;	/* interpolated value   	 */

   /* allocate neighbors array */

	neighbors = (struct neighbor *) ecalloc (nclosest,
						  sizeof(struct neighbor));
	if (neighbors == NULL) {
		error ("error allocating neighbors array");
	}


/* normal (non-crossval) interpolation loop */

  if (jackflag == 0) {  

	nlines = hnlines (fdo);
	nsamps = hnsamps (fdo);

   /* allocate mask buffer */

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

	northing = bline;
	easting = bsamp;

   /* loop interpolating and writing complete lines of data */


	for (line = 0; line < nlines; line++) {

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image, line %d", 
					line);
			}
		}

		obufp = obuf;
		mbufp = mbuf;

		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp++) {

				/* find closest neighbors */

				find_neighbors (northing, easting, npoints, 
					points, nclosest, neighbors,
					0.0);

				/* interpolate */

				*obufp++ = interp (nclosest, neighbors);

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

    } else {    /****************** crossval routine ****************/

	/* open ascii outfile */

 	if( (outfile = fopen(jackfile, "w")) == NULL) {
		error("can't open %s for writing", jackfile);	
	}

	for (i=0; i < npoints; i++) {

		/* find closest neighbors */

		if (globalpts[i].val == nodataval) {
		  fprintf(outfile,"%s %s %.2f %.2f %.2f\n", 
		      globalpts[i].e_str, globalpts[i].n_str, 
		      nodataval, nodataval, nodataval);
		continue;
		}


		find_neighbors (globalpts[i].north, globalpts[i].east, npoints,
			points, nclosest, neighbors, globalpts[i].val);

		/* interpolate */

		jackval = interp (nclosest, neighbors);


		fprintf(outfile,"%s %s %s %f %f\n", globalpts[i].e_str, 
		   globalpts[i].n_str, globalpts[i].v_str, jackval, 
		   globalpts[i].val - jackval);

	}
    } /* end if-else */
}
