/*
 **
 ** NAME
 ** 	iwbt -- compute wet or icebult from air and dewpoint temperatures
 **
 ** SYNOPSIS
 **
 **	void iwbt (fdi, fdm, fdo)
 **	int fdi, fdm, fdo;
 **
 ** DESCRIPTION
 ** 	iwbt reads a three-band image of air and dewpoint temperature and
 **	elevation and computes wetbulb temperature;
 **	Elevation is used to compute pressure using the hydrostatic equation
 **	air and dewpoint temperature, and air pressure are used to compute
 **	wet or ice bult temperature using the psychrometric constant, its
 **	value at specific temperatures using the Clausius-Clapeyron equation
 **	solving for Tw or Ti using the Newton-Raphson iterative approximation;
 **	Returns Tw or Ti, to the given output image;
 **	Works at all temperatures (accounting for both fusion and vaporization).
 **
 */

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "fpio.h"
#include "bih.h"
#include "pixio.h"

void
iwbt (
		int             fdi,		/* input image file descriptor	*/
		int             fdm,		/* mask image file descriptor	*/
		int             fdo)		/* output image file descriptor	*/
{
	pixel_t        *ibuf;		/* -> input line buffer		*/
	pixel_t        *mbuf =NULL;	/* -> mask line buffer		*/
	fpixel_t       *obuf;		/* -> output line buffer	*/
	pixel_t        *ibufp;		/* -> in input buffer		*/
	pixel_t        *mbufp;		/* -> in mask buffer		*/
	fpixel_t       *obufp;		/* -> in output buffer		*/
	fpixel_t      **fmap;		/* -> all input F.P. maps	*/
	fpixel_t       *map0;		/* -> band 0 F.P. map		*/
	fpixel_t       *map1;		/* -> band 1 F.P. map		*/
	fpixel_t       *map2;		/* -> band 2 F.P. map		*/
	int	       *fmaplen;	/* -> lengths of all F.P. maps	*/
	int		maplen;		/* length of band 0 F.P. map	*/
	int		fdt;		/* file desc for temp file	*/
	int		nbytes;		/* # bytes in temp file buffer	*/
	int		nlines;		/* # lines in input image	*/
	int		nsamps;		/* # samples in input image	*/
	int		line;		/* line loop counter		*/
	int		samp;		/* samp loop counter		*/
	int		pixel;		/* pixel loop counter		*/
	double		td;		/* dew point temperature (C)	*/
	double		tw;		/* wet bulb temperature (C)	*/
	double		ta;		/* air temperature (C)		*/
	double		z;		/* elevation (m)		*/
	double		pa;		/* air pressure (pa)		*/
	double		min =0.0;	/* min RH or VP in output	*/
	double		max =0.0;	/* max RH or VP in output	*/
	double         *satvp;		/* lookup table for sat VP	*/
	char	       *tempfile;	/* name of temp output file	*/
	bool_t		first_pixel;	/* flag for first pixel		*/


	nsamps = hnsamps (fdi);
	nlines = hnlines (fdi);
	nbytes = nsamps * sizeof(fpixel_t);


	/* open temporary output file */

	tempfile = mktemplate("iwbt");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

	/* allocate buffer for input image line */

	ibuf = (pixel_t *) ecalloc (nsamps * IBANDS, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("Unable to allocate input line buffer");
	}

	/* allocate buffer for mask image line */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("Unable to allocate mask line buffer");
		}
	}

	/* allocate buffer for output image line */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("Unable to allocate output line buffer");
	}

	/* access input image F.P. maps */

	fmap = fpmap (fdi);
	map0 = fmap[0];
	map1 = fmap[1];
	map2 = fmap[2];
	fmaplen = fpmaplen (fdi);
	maplen = fmaplen[1];

	first_pixel = TRUE;

	/* loop on image lines, performing calculations */

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("Error reading input image, line %d", line+1);
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("Error reading mask image, line %d", line+1);
			}
		}

		ibufp = ibuf;
		obufp = obuf;
		mbufp = mbuf;

		/* convert elevation to air pressure for each sample in line */

		for (samp = 0; samp < nsamps; samp++) {

			/*	set vars	*/
			z = map0[*ibufp++];
			ta = map1[*ibufp++];
			td = map2[*ibufp++];

			/* if not a masked point */
			if (fdm == ERROR || mbufp++) {

				/*	set pa	*/
				if (z == 0.0) {
					pa = SEA_LEVEL;
				}
				else {
					pa = HYSTAT (SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
							(z / 1000.0), GRAVITY, MOL_AIR);
				}

				/*	convert ta & td to Kelvin	*/
				ta += FREEZE;
				td += FREEZE;

				/*	call wetbulb function & fill output buffer	*/
				tw = wetbulb(ta, td, pa);
				*obufp = tw - FREEZE;

				if (first_pixel) {
					first_pixel = FALSE;
					min = *obufp;
					max = *obufp;
				} else {
					min = MIN (min, *obufp);
					max = MAX (max, *obufp);
				}
				obufp++;
//				mbufp++;
			}

		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

	/* create/write LQH for output image */

	newlqh (fdo, min, max);

	/* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);
}
