/*
** NAME
**      idewpt - convert vapor pressure to dew point temperature
**
** SYNOPSIS
**	void idewpt (fdi, fdm, fdo)
**	int fdi;
**	int fdm;
**	int fdo;
** 
** DESCRIPTION
**      idewpt converts vapor pressure to dew point temp
** 
** HISTORY
**	10/21/92: Written by D. Marks, USGS, EPA, ERL/C;
**
*/

#include        <math.h>
#include        "ipw.h"
#include        "bih.h"
#include        "pixio.h"
#include        "fpio.h"
#include        "envphys.h"
#include	"pgm.h"

void
idewpt (
	int	fdi,		/* input image file descriptor		*/
	int	fdm,		/* mask image file descriptor		*/
	int	fdo)		/* output image file descriptor		*/
{
	float	dpt;		/* dew point temp			*/
        float	ea;		/* vapor pressure			*/
        float	dpt_min =0.0;	/* min dpt in output image		*/
        float	dpt_max =0.0;	/* max dpt in output image		*/

	fpixel_t       *ibuf;		/* pointer to input buffer	*/
	fpixel_t       *ibufp;		/* pointer in input buffer	*/
	fpixel_t       *obuf;		/* pointer to output buffer	*/
	fpixel_t       *obufp;		/* pointer in output buffer	*/
	pixel_t	       *mbuf;		/* pointer to mask buffer	*/
	pixel_t	       *mbufp;		/* pointer in mask buffer	*/
	int		nsamps;		/* # samples per line		*/
	int		nlines;		/* # lines in image		*/
	int		samp;		/* sample loop counter		*/
	int		line;		/* line loop counter		*/
	int		first_pixel;	/* flag for first pixel proc`ed */
	int		fdt;		/* temp output file desc	*/
	int		nbytes;		/* #bytes to write		*/

	char	       *tempfile;	/* temporary output file	*/

	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;


   /* open temporary output file */

	tempfile = mktemplate("idewpt");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (nsamps * IBANDS, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate input buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	first_pixel = TRUE;

	
   /* read input data and do calculations */

	for (line=0; line < nlines; line++) {

		/* read line of image */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("read error, line %d", line);
		}

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* Do calculations on each sample of line */

		ibufp = ibuf;
		mbufp = mbuf;
		obufp = obuf;

		for (samp=0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp++) {

				/* extract input band */

				ea = *ibufp++;

				/*	calculate dew point temp	*/

				dpt = (float) dew_point((double) ea);

				/*	convert from K to C	*/

				dpt -= FREEZE;

				/* set output band */

				*obufp++ = dpt;

				/* update min/max */

				if (!first_pixel) {
					dpt_min = MIN (dpt_min, dpt);
					dpt_max = MAX (dpt_max, dpt);
				} else {
					first_pixel = FALSE;
					dpt_min = dpt;
					dpt_max = dpt;
				}

			} else {
				*obufp++ = 0.0;
				ibufp += IBANDS;
			}
		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

   /* create/write LQH for output image */

	newlqh (fdo, dpt_min, dpt_max);

   /* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);

}
