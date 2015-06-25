/*
 ** NAME
 **      topotherm - calculate thermal radiation from the atmosphere
 **
 ** SYNOPSIS
 **	void topotherm (fdi, fdm, fdv, fdc, fdo)
 **	int fdi;
 **	int fdm;
 **	int fdv;
 **	int fdo;
 **
 ** DESCRIPTION
 **	topotherm calculates thermal radiation from the atmosphere corrected
 **	for topographic effects, from near surface air temperature Ta,
 **	dew point temperature DPT, and elevation.
 **	Based on a model by Marks and Dozier, "A clear-sky longwave radiation
 **	model for remote alpine areas" (in: Archiv fur Meteorologie,
 **	Geophysik und Bioklimatologie, Series B vol 27, no 23, pp 159-187, 1979).
 **	Contribution from canopy is estimated from canopy shading using Beers'
 **	Law (Monteith and Unsworth, 1990), after Wigmosta, Vail, and Lettenmaier,
 **	"A distributed hydrology-vegetation model for complex terrain", Water
 **	Resources Research, vol 30, no 6, pp 1665-1679, 1994.
 **
 ** HISTORY
 **	19/10/92 Written by Danny Marks, US Geological Survey,
 **		 EPA Environmental Research Laboratory, Corvallis OR
 **	13/09/95 Corrections for terrain view factory and vegetation view
 **		 factor added by D. Marks, US Geological Survey,
 **		 EPA Environmental Research Laboratory, Corvallis OR
 **	17/02/00 Corrections for cloud attenuation factor added, and
 **		 vegetation view factor changed to vegetation attenuation
 **		 factor by D. Marks, USDA-ARS, NWRC, Boise ID
 **
 */

#include        <math.h>
#include        "ipw.h"
#include        "bih.h"
#include        "pixio.h"
#include        "fpio.h"
#include        "envphys.h"
#include	"radiation.h"
#include	"pgm.h"

void
topotherm (
		int	fdi,		/* input image file descriptor		 */
		int	fdm,		/* mask image file descriptor		 */
		int	fdv,		/* veg atten image file descriptor	 */
		int	fdc,		/* cloud atten image file descriptor	 */
		int	fdo)		/* output image file descriptor		 */
{
	float	ta;		/*	air temp		*/
	float	tw;		/*	dew point		*/
	float	z;		/*	ref. elev		*/
	double	ea;		/*	vapor pressure		*/
	float	skvfac;		/*	sky view factor		*/
	float	vegfac;		/*	veg view factor		*/
	float	cloudfac;	/*	cloud view factor	*/
	float	emiss;		/*	atmos. emiss.		*/
	float	lw_in;		/*	lw irradiance		*/
	float	press;		/*	air pressure		*/
	float	T0;		/*	Sea Level ta		*/
	float	lw_min;         /* min lw_in in output image    */
	float	lw_max;         /* max lw_in in output image    */

	fpixel_t       *ibuf;		/* pointer to input buffer	 */
	fpixel_t       *ibufp;		/* pointer in input buffer	 */
	fpixel_t       *vbuf;		/* pointer to veg fact buffer	 */
	fpixel_t       *vbufp;		/* pointer in veg fact buffer	 */
	fpixel_t       *cbuf;		/* pointer to cloud fact buffer	 */
	fpixel_t       *cbufp;		/* pointer in cloud fact buffer	 */
	fpixel_t       *obuf;		/* pointer to output buffer	 */
	fpixel_t       *obufp;		/* pointer in output buffer	 */
	pixel_t	       *mbuf;		/* pointer to mask buffer	 */
	pixel_t	       *mbufp;		/* pointer in mask buffer	 */
	int		nsamps;		/* # samples per line		 */
	int		nlines;		/* # lines in image		 */
	int		samp;		/* sample loop counter		 */
	int		line;		/* line loop counter		 */
	int		first_pixel;	/* flag for first pixel proc`ed  */
	int		fdt;		/* temp output file desc	 */
	int		nbytes;		/* #bytes to write		 */

	char	       *tempfile;	/* temporary output file	 */

	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;


	/* open temporary output file */

	tempfile = mktemplate("topotherm");
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
			error ("can't allocate mask buffer");
		}
	} else {
		mbuf = NULL;
	}

	/* Allocate veg fact buffer */

	if (fdv != ERROR) {
		vbuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
		if (vbuf == NULL) {
			error ("can't allocate veg fact buffer");
		}
	} else {
		vbuf = NULL;
	}

	/* Allocate cloud fact buffer */

	if (fdc != ERROR) {
		cbuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
		if (cbuf == NULL) {
			error ("can't allocate cloud fact buffer");
		}
	} else {
		cbuf = NULL;
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

		/* read line of veg fact image */

		if (fdv != ERROR) {
			if (fpvread (fdv, vbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* read line of cloud fact image */

		if (fdc != ERROR) {
			if (fpvread (fdc, cbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* Do calculations on each sample of line */

		ibufp = ibuf;
		mbufp = mbuf;
		vbufp = vbuf;
		cbufp = cbuf;
		obufp = obuf;

		for (samp=0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp++) {

				/* extract input bands */

				ta = *ibufp++;
				tw = *ibufp++;
				z = *ibufp++;
				skvfac = *ibufp++;
				if (fdv != ERROR) {
					vegfac = *vbufp++;
				}
				if (fdc != ERROR) {
					cloudfac = *cbufp++;
				}
				/* convert ta and tw from C to K */
				ta += FREEZE;
				tw += FREEZE;

				if(tw<0.0 || ta<0.0) {
					error ("bad input temps: ta= %g, dpt=%g",
							ta, tw);
				}

				/*	calculate theoretical sea level	*/
				/*	atmospheric emissivity  */
				/*	from reference level ta, tw, and z */

				if(tw > ta) {
					if ((tw - ta) > 1.0) {
						warn ("dpt=%g > ta; tw set to ta",tw);
					}
					tw = ta;
				}

				ea = sati((double) tw);
				emiss = (float) brutsaert((double)ta,
						STD_LAPSE_M, ea,
						(double)z, SEA_LEVEL);

				/*	calculate sea level air temp	*/

				T0 = ta - (z * STD_LAPSE_M);

				/*	adjust emiss for elev, terrain	*/
				/*	     veg, and cloud shading	*/

				press = HYSTAT(SEA_LEVEL, T0,
						STD_LAPSE, (z/1000.),
						GRAVITY, MOL_AIR);

				/* elevation correction */
				emiss *= press/SEA_LEVEL;

				/* terrain factor correction */
				emiss = (emiss * skvfac)+(1.0 - skvfac);

				/* veg factor correction */
				if (fdv != ERROR)
					emiss = (emiss * vegfac)+(1.0 - vegfac);

				/* cloud factor correction */
				if (fdc != ERROR)
					emiss = (emiss * cloudfac)+(1.0 - cloudfac);

				/* check for emissivity > 1.0 */
				if (emiss > 1.0)
					emiss = 1.0;

				/*	calculate incoming lw rad	*/

				lw_in = emiss * STEF_BOLTZ *ta*ta*ta*ta;

				/* set output band */

				*obufp++ = lw_in;

				/* update min/max */

				if (!first_pixel) {
					lw_min = MIN (lw_min, lw_in);
					lw_max = MAX (lw_max, lw_in);
				} else {
					first_pixel = FALSE;
					lw_min = lw_in;
					lw_max = lw_in;
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

	newlqh (fdo, lw_min, lw_max);

	/* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);

}
