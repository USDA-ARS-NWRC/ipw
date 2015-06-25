/*
** NAME
** 	iimport -- import ASCII file to IPW image file
** 
** SYNOPSIS
**	void iimport (ifp, fdo, nlines, nsamps, nbands);
**	FILE *ifp;
**	int fdo;
**	int nlines, nsamps, nbands;
** 
** DESCRIPTION
** 	Import reads the given ASCII data file and writes the output IPW
**	image file.
** 
*/

#include "ipw.h"
#include "pgm.h"

void
iimport (
	FILE	       *ifp,		/* file ptr to input file	 */
	int		fdo,		/* output image file descriptor	 */
	int		nlines,		/* # lines in output image	 */
	int		nsamps,		/* # samples in output image	 */
	int		nbands)		/* # bands in output image	 */
{
	int		nread;		/* # values read in fscanf()	 */
	int		nbytes;		/* # bytes to write		 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		band;		/* band loop counter		 */
	int		fdt;		/* file desc for temp file	 */
	int		first_pixel;	/* flag for first pixel read	 */
	float		dummy;		/* dummy var for EOF test	 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	char	       *tempfile;	/* name of temporary file	 */
	float	       *min;		/* min value for each band	 */
	float	       *max;		/* max value for each band	 */
	float	       *flar;		/* holding for scanned values	 */
	float          *flar_p;         /* pointer into flar		 */


   /* get temporary filename and open file */

	tempfile = mktemplate("iimport");
	if (tempfile == NULL) {
		error ("can't create temporary filename");
	}

	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("can't open temporary file");
	}

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * nbands, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	flar = (float *) ecalloc (nsamps * nbands, sizeof(float));
	if (flar == NULL) {
		error ("can't allocate flar array");
	}

   /* allocate min and max arrays */

	min = (float *) ecalloc (nbands, sizeof(float));
	if (min == NULL) {
		error ("can't allocate min array");
	}

	max = (float *) ecalloc (nbands, sizeof(float));
	if (max == NULL) {
		error ("can't allocate max array");
	}

	first_pixel = TRUE;
	nbytes = nsamps * nbands * sizeof(fpixel_t);

   /* loop reading and writing complete lines of data */

	for (line = 0; line < nlines; line++) {
	  obufp = obuf;
	  /* quickly read in nbands * nsamps floating point values */
	  if ((nread = ffscanf(ifp, nbands*nsamps, flar)) != nbands*nsamps) {
	    if (nread == EOF) {
	      error ("read past EOF, line %d", line);
	    } else {
	      error ("error reading input file, line %d", line);
	    }
	  }
	  flar_p = flar;
	  for (samp = 0; samp < nsamps; samp++) {
	    /* find min and max, and move to obuf */
	    for (band = 0; band < nbands; band++) {
	      *obufp = *flar_p++;
	      /* keep min and max of data */
	      if (!first_pixel) {
	        min[band] = MIN (*obufp, min[band]);
	        max[band] = MAX (*obufp, max[band]);
	      } else {
	        min[band] = *obufp;
	        max[band] = *obufp;
	        if (band == (nbands - 1))
	          first_pixel = FALSE;
	      }
	      obufp++;
	    }
	  }
	  /* write output line */
	  if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
		error ("error writing temporary file, line %d", line);
	  }
	}

	free(flar);

   /* attempt another read to test for EOF */

	if ((nread = fscanf (ifp, "%f", &dummy)) == 1) {
		warn ("input file contains more than %d values - remainder ignored",
			nlines * nsamps * nbands);
	}

   /* close temporary file */

	(void) uclose (fdt);

   /* create/write LQH quantized with mins/maxs */

	newlqh (fdo, nbands, min, max);

   /* copy temporary file to output image */

	output (tempfile, fdo, nbands);

	SAFE_FREE(tempfile);
	SAFE_FREE(obuf);
	SAFE_FREE(min);
	SAFE_FREE(max);
}
