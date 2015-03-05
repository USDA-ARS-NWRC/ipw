/*
** NAME
** 	reproj -- reproject input image to output image
** 
** SYNOPSIS
**	void
**	reproj (fdi, fdm, fdo, resamp, iproj, i_bline, i_bsamp, i_dline, i_dsamp,
**		                       oproj, o_bline, o_bsamp, o_dline, o_dsamp)
**	int fdi, fdm, fdo;
**	int resamp;
**	struct proj *iproj;
**	double i_bline, i_bsamp, i_dline, i_dsamp;
**	struct proj *oproj;
**	double o_bline, o_bsamp, o_dline, o_dsamp;
** 
** DESCRIPTION
**	reproj reads the input image, reprojects and resamples the data and
**	write the output image.
**
**	Algorithm:
**	    Read the entire input image into a 2-dimensional array
**	    If a mask image is supplied, read the mask file line by line
**	        setting the "masked" pixels in the input image array to NO_DATA
**	    For each line in the output image
**		For each sample in the output image
**		    Call GCTP to reproject output image projection coordinates
**			to input image projeciton coordinates for this output
**			pixel
**		    Convert input image projection coordinates to decimal
**			line, sample coordinates in input image
**		    Interpolate for input image value at calculated line
**			and sample using specified resampling technique
**			(currently either nearest neighbor or bilinear interp.)
**		    Set output image pixel to interpolated value
**		Write output line to output image
** 
*/

#include <time.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "gctp.h"
#include "pgm.h"

void
reproj(
	int             fdi,		/* input image file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output image file descriptors */
	int		resamp,		/* resampling technique flag	 */
	struct projdef *iproj,		/* input projection parms	 */
	double		i_bline,	/* input image UL geo line coord */
	double		i_bsamp,	/* input image UL geo samp coord */
	double		i_dline,	/* input image geo line incr	 */
	double		i_dsamp,	/* input image geo samp incr	 */
	struct projdef *oproj,		/* output projection parms	 */
	double		o_bline,	/* output image UL geo line coord*/
	double		o_bsamp,	/* output image UL geo samp coord*/
	double		o_dline,	/* output image geo line incr	 */
	double		o_dsamp)	/* output image geo sample incr  */
{
	int		nbands;		/* # bands in images		 */
	int		i_nlines;	/* # lines in input image	 */
	int		i_nsamps;	/* # samples in input image	 */
	int		o_nlines;	/* # lines in output image	 */
	int		o_nsamps;	/* # samples in output image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		indef[3];	/* input proj ID/zone/units	 */
	int		outdef[3];	/* output proj ID/zone/units	 */
	int		errflg;		/* error return from gtrnz0	 */
	float		indline;	/* reprojected line index	 */
	float		indsamp;	/* reprojected sample index	 */
	double		incoord[2];	/* x,y coords for input image	 */
	double		outcoord[2];	/* x,y coords for output image	 */
	pixel_t       **ibuf;		/* input buffer (entire file)	 */
	pixel_t        *ibufp;		/* -> in input buffer		 */
	pixel_t        *obuf;		/* output line buffer		 */
	pixel_t        *obufp;		/* -> in output line buffer	 */
	pixel_t	       *mbuf;		/* mask buffer			 */
	pixel_t	       *mbufp;		/* -> in mask buffer		 */
	FILE	       *errfile;


	nbands = hnbands (fdi);
	i_nlines = hnlines (fdi);
	i_nsamps = hnsamps (fdi);
	o_nlines = hnlines (fdo);
	o_nsamps = hnsamps (fdo);

   /* initialize projection definition arrays */

	indef[0] = iproj->id;
	indef[1] = iproj->zone;
	indef[2] = iproj->uid;
	outdef[0] = oproj->id;
	outdef[1] = oproj->zone;
	outdef[2] = oproj->uid;
	errfile = NULL;

   /* allocate buffer for entire input file */

	ibuf = (pixel_t **) allocnd (sizeof(pixel_t), 2, i_nlines,
				      i_nsamps * nbands);
	if (ibuf == NULL) {
		error ("error allocating input buffer");
	}

   /* allocate mask line buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (i_nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("error allocating mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* read entire input file and apply mask */

	for (line = 0; line < i_nlines; line++) {

		/* read input line */

		if (pvread (fdi, ibuf[line], i_nsamps) != i_nsamps) {
			error ("error reading input image, line %d", line);
		}

		/* read mask line and mask out pixels */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, i_nsamps) != i_nsamps) {
				error ("error reading mask image, line %d", line);
			}
			ibufp = ibuf[line];
			mbufp = mbuf;
			for (samp = 0; samp < i_nsamps; samp++) {
				if (*mbufp == 0) {
					*ibufp = NODATA;
				}
				ibufp++;
				mbufp++;
			}
		}
	}

   /* free mask buffer */

	if (fdm != ERROR)
		free ((char *) mbuf);

   /* allocate output line buffer */

	obuf = (pixel_t *) ecalloc (o_nsamps * nbands, sizeof(pixel_t));
	if (obuf == NULL) {
		error ("error allocating output line buffer");
	}

   /* loop on output lines and samples */
   /* reproject to get line, sample in input image */
   /* get data value at pixel using specified resampling technique */

	outcoord[0] = o_bsamp;
	outcoord[1] = o_bline;

	for (line = 0; line < o_nlines; line++) {

		obufp = obuf;

		for (samp = 0; samp < o_nsamps; samp++) {

			/* get input image coordinates for output pixel */


			/* call GCTP to transform coordinates */

			gtrnz0_ (outcoord, outdef, oproj->parms, incoord,
				 indef, iproj->parms, errfile, &errflg);

   			/* process error if any */

			if (errflg != 0) {
				error ("error projecting line %d sample %d",
					line, samp);
			}

			/* convert to input image line, sample indices */

			indline = (incoord[1] - i_bline) / i_dline;
			indsamp = (incoord[0] - i_bsamp) / i_dsamp;

			/* get resampled data point */

			switch (resamp) {
				case NN:
					*obufp++ = nearest (ibuf, indline,
							indsamp, i_nlines,
							i_nsamps);
					break;
				case BI:
					*obufp++ = bilinear (ibuf, indline,
							indsamp, i_nlines,
							i_nsamps);
					break;
				case CC:
					*obufp++ = cubic (ibuf, indline,
							indsamp, i_nlines,
							i_nsamps);
					break;
				case XX:
					*obufp++ = nothing (ibuf, indline,
							indsamp, i_nlines,
							i_nsamps);
					break;
			}

			outcoord[0] += o_dsamp;
		}

		/* write line of output image */

		if (pvwrite (fdo, obuf, o_nsamps) != o_nsamps) {
			error ("error writing output image, line %d", line);
		}

		outcoord[0] = o_bsamp;
		outcoord[1] += o_dline;
	}
}
