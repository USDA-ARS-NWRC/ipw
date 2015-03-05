/*
** NAME
** 	resamp -- read input image, resample and write output image
** 
** SYNOPSIS
**	void
**	resamp (fdi, fdm, fdo, zline, zsamp, ztype)
** 	int fdi, fdm, fdo;
**	int zline, zsamp;
**	int ztype;
** 
** DESCRIPTION
**	resamp reads the input image data, averages or takes the minimum or
**	maximum of the zline lines and zsamp samples of the input image
**	image that correspond to an output pixel of the output image, and
**	write the output image.
**
*/

#include <string.h>

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
resamp (
	int		fdi,		/* input image file descriptor	 */
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output image file descriptor	 */
	int		zline,		/* line zoom factor		 */
	int		zsamp,		/* sample zoom factor		 */
	int		ztype)		/* zoom type: MIN,MAX,or MEAN	 */
{
	int		nbands;		/* # bands in input image	 */
	int		i_nsamps;	/* # samples in input image	 */
	int		i_nlines;	/* # lines in input image	 */
	int		o_nsamps;	/* # samples in output image	 */
	int		o_nlines;	/* # lines in output image	 */
	int		iline;		/* counts lines to next output	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int	       *npix;		/* # pixels in mean for out pix	 */
	fpixel_t       *ibuf;		/* input buffer			 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	pixel_t        *mbuf;		/* mask buffer			 */
	double	       *sums;		/* buffer to hold pixel sums	 */
	

	nbands = hnbands (fdo);
	i_nlines = hnlines (fdi);
	i_nsamps = hnsamps (fdi);
	o_nlines = hnlines (fdo);
	o_nsamps = hnsamps (fdo);


   /* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (i_nsamps * nbands, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input image line buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (i_nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask line buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate output buffer */

	obuf = (fpixel_t *) ecalloc (o_nsamps * nbands, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output image line buffer");
	}

   /* Allocate buffer to hold sums for mean computation */

	if (ztype == MEAN_VAL) {
		sums = (double *) ecalloc (o_nsamps * nbands, sizeof(double));
		if (sums == NULL) {
			error ("can't allocate sums line buffer");
		}
	} else {
		sums = NULL;
	}

   /* Allocate buffer to hold pixel count */

	npix = (int *) ecalloc (o_nsamps, sizeof(int));
	if (npix == NULL) {
		error ("can't allocate pixel count line buffer");
	}

   /* loop on input lines */

	iline = 0;

	for (line = 0; line < i_nlines; line++) {

		/* read input image line */

		if (fpvread (fdi, ibuf, i_nsamps) != i_nsamps) {
			error ("error reading input image line %d", line);
		}

		/* read mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, i_nsamps) != i_nsamps) {
				error ("error reading mask image line %d", line);
			}
		}

		/* if first line in average, initialize to NO_DATA value */

		if (iline == 0) {
			obufp = obuf;
			for (samp = 0; samp < o_nsamps * nbands; samp++) {
				*obufp++ = NO_DATA;
			}
			memset (npix, 0, o_nsamps * sizeof(int));
		}

		/* Accumulate sums or update mins or maxs */

		resamp_line (ibuf, fdm, mbuf, i_nsamps, obuf, sums, npix,
			     o_nsamps, nbands, zsamp, ztype);

		/* output image line after every <zline> lines */

		iline++;
		if (iline >= zline) {
			out_line (fdo, obuf, sums, npix, o_nsamps, nbands, ztype);
			iline = 0;
		}
	}

	/* write last output line, if necessary */

	if (iline > 0) {
		out_line (fdo, obuf, sums, npix, o_nsamps, nbands, ztype);
	}


	SAFE_FREE(ibuf);
	SAFE_FREE(obuf);
	SAFE_FREE(mbuf);
	SAFE_FREE(sums);
	SAFE_FREE(npix);
}
