/*
** name
**	ipmx -- calculate and write out values of PE
**
** synopsis
**	void ipmx (fdi, fdm, fdo)
**	int fdi;	 { file descriptor for input image 	}
**	int fdm;	 { file descriptor for mask image 	}
**	int fdo;	 { file descriptor for output image 	}
**
** description
**	Ipmx calculates LE flux (W/m2, +/-) using the Penman-Monteith
**	approach (Monteith and Unsworth 1990). The program reads
**      a 6-band image file containing the following daily data:
**
**		band	contents
**		----    --------
**		0	air temperature (C)
**		1	actual vapor pressure at air temperature (Pa)
**              2	wind speed (m/s)
**		3	net radiation (W/m^2, negative away from surface)
**		4	projected (1-sided) leaf area index
**		5	bulk canopy conductivity (m/s)
**
**	If no input file is specified, the programs reads from the input
**	image from standard input.  The program outputs a 4-band image
**	to standard output:
**
**		band	contents
**		----    --------
**		0	latent energy flux (W/m^2)
**		1	proportion of LE due to radiation (W/m^2)
**              2	proportion of LE due to vapor pressure deficit (W/m^2)
**		3	aerodynamic resistance (s/m)
**
**	Assumptions:
**
**	No stability correction, G is estimated, canopy height and z0 are
**	a function of lai.
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "envphys.h"
#include "pgm.h"
 
#define VK 0.41         /* von karmen const. */

void
ipmx(
	int             fdi,		/* input file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
					/*   (== ERROR if no mask file)  */
	int             fdo)		/* output file descriptor	 */
{
	char	       *tempfile;	/* name of temporary file	 */
	int		fdt;		/* file desc for temp file	 */

	int             nlines;		/* # lines in input image	 */
	int             nsamps;		/* # samples in input image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		nbytes;		/* # bytes in output buffer	 */
	int		first_pixel;	/* flag for first pixel proc`ed */

	fpixel_t       *ibuf;		/* input buffer			 */
	pixel_t        *mbuf;		/* mask buffer			 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *ibufp;		/* -> ibuf			 */
	pixel_t        *mbufp;		/* -> mbuf			 */
	fpixel_t       *obufp;		/* -> obuf			 */
 
	double		vector[IBANDS]; /* input image vector		 */

	double 		ta;
	double 		ea;
	double 		u;
	double 		Rn;
	double 		lai;
	double 		cond;
	double 		z0;
	double 		h0;
	double 		d0;
	double 		z;
	double 		ra;
	double 		arad;
	double 		tak;
	double 		t1;
	double 		t2;
	double 		sat_vpres;
        double 		sat_vpres1;
        double 		sat_vpres2;
	double 		slope;
	double 		gamma;
	double 		rc;
	double 		gammas;
	double 		vpd;
	double 		p;
	double 		radflx;
	double 		vpdflx;
	double 		le;

	double		mins[OBANDS];	/* mins for bands in output image */
	double		maxs[OBANDS];	/* maxs for bands in output image */


	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);
	nbytes = nsamps * OBANDS * sizeof(fpixel_t);

   /* open temporary output file */

	tempfile = mktemplate("iptx");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* allocate buffers */

	ibuf = (fpixel_t *) ecalloc (nsamps * IBANDS, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask buffer");
		}
	}
	else {
		mbuf = NULL;
	}

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

   /* loop reading data & performing calculations on each sample */

	first_pixel = TRUE;

	for (line = 0; line < nlines; line++) {

		/* read line of input image */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line);
		}

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image, line %d", line);
			}
		}

		ibufp = ibuf;
		mbufp = mbuf;
		obufp = obuf;

		for (samp = 0; samp < nsamps; samp++) {

			if (get_sample (fdi, ibufp, mbufp, vector, IBANDS)
				== IBANDS) {

				ta   = vector[0];
				ea   = vector[1];
				u    = vector[2];
				Rn   = vector[3];
				lai  = vector[4];
				cond = vector[5];

				if (lai <= 1.0) {

					/* calc zero plane displacement, d0 */

				        z0 = 0.01 * 1.0;
					h0 = 7.35 * z0;	  /* Brutsaert p. 113 */
					d0 = h0 * (2/3);  /* Brutsaert p. 116 */

					/* calculate aerodynamic resistance,
					   ra (Stewart, 1984 p.6 ) */

					z = 2.0 * 1.0;
					  /* set measurement height of 2 m */

					ra = ((log((z-d0)/z0))
						* (log((z-d0)/z0)))
						/ (((VK*VK)*u));

					arad = (Rn - (0.1 * Rn));
					  /* Campbell 1977, p. 138 */
				}
				else {
					ra = 50.0 / lai;
					  /* Running BGC */

					arad = Rn * 1.0;
					  /* Campbell 1977, p. 138 */
				}

				/* convert temperature to kelvin */

				tak = ta+273.15;
				t1 = tak+0.5;
				t2 = tak-0.5;

				/* calculate saturation vapor pressure */

			        sat_vpres = sati(tak); 
				sat_vpres1 = sati(t1); 
				sat_vpres2 = sati(t2); 

				/* calculate slope of saturation vapor
				   pressure/temperature curve */

				slope = (sat_vpres1 - sat_vpres2) / 100;

				/* calculate gamma */

				gamma = 0.646 + 0.0006 * ta;

				/* calculate gammas */

				if (cond > 0.0) {
					rc = 1.0 / cond;
				}
   				else {
					rc = 1.0*0.0;
   				}
				gammas = (gamma * (ra + rc)) / ra;

				/* calculate LE */

				vpd = (sat_vpres/100) - (ea/100);
				   /* vapor pressure deficit mb */
				if(vpd < 0) {
					vpd = 0;
				}

				p = (1.292 - 0.00428 * ta);
				  /* air density, kg/m3 */

				/* proportion of LE contributed by
				   radiation term */

				radflx = (slope * (-1*(arad))) / (slope+gammas);

				/* proportion of LE contributed by
				   the vapor pressure deficit */
				vpdflx = ((p * CP_AIR * -1 * (vpd)) / ra)
					 / (slope + gammas);

				le = radflx + vpdflx;
 
				*obufp++ = le;
				*obufp++ = radflx;
				*obufp++ = vpdflx;
				*obufp++ = ra;

				/* update min/max */

				if (first_pixel) {
					first_pixel = FALSE;
					mins[0] = maxs[0] = le;
					mins[1] = maxs[1] = radflx;
					mins[2] = maxs[2] = vpdflx;
					mins[3] = maxs[3] = ra;
				} else {
					mins[0] = MIN (mins[0], le);
					maxs[0] = MAX (maxs[0], le);
					mins[1] = MIN (mins[1], radflx);
					maxs[1] = MAX (maxs[1], radflx);
					mins[2] = MIN (mins[2], vpdflx);
					maxs[2] = MAX (maxs[2], vpdflx);
					mins[3] = MIN (mins[3], ra);
					maxs[3] = MAX (maxs[3], ra);
				}

			} else {
				int band;

				for (band = 0; band < OBANDS; band++)
					*obufp++ = NO_DATA;
			}

		ibufp += IBANDS;
		if (mbuf != NULL)
			mbufp++;
		}  /* for each sample */

		/* write output buffer to temporary file */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("error writing output buffer to temp file, line %d",
				 line);
		}
	}  /* for each line */

   /* close temporary file */

	(void) uclose (fdt);

	free (ibuf);
	free (obuf);

   /* create new LQH header from min and max */

	newlqh (fdo, mins, maxs);

   /* copy temporary file to output image */

	output (tempfile, fdo);
}
