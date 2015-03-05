/*
** NAME
**	gelevrad -- calculate up and write out values of beam and diffuse
**
** SYNOPSIS
**	gelevrad (fdi, fdm, fdo, zelev, omega, gfact, S0)
**	int fdi, fdm, fdo;
**	double zelev, omega, gfact, S0;
**
** DESCRIPTION
**	Gelevrad reads RAW pixel_t elevation values, applies the look-up
**	tables beam and diffuse, and writes the fpixel_t output image.
**
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
gelevrad (
	int             fdi,		/* input file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output file descriptor	 */
	double          zelev,		/* elevation at tau measurement	 */
	double		omega,		/* single-scattering albedo	 */
	double		gfact,		/* scattering asymmetry parameter*/
	double          S0)		/* exoatmos solar irrad		 */
{
	char	       *tempfile;	/* name of temporary file	 */
	int		fdt;		/* file desc for temp file	 */
	int             nlines;		/* # lines in input image	 */
	int             nsamps;		/* # samples in input image	 */
	int		nbytes;		/* # bytes in output buffer	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	fpixel_t       *ibuf;		/* input buffer			 */
	pixel_t        *mbuf =NULL;	/* mask buffer			 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *ibufp;		/* -> ibuf			 */
	pixel_t        *mbufp;		/* -> mbuf			 */
	fpixel_t       *obufp;		/* -> obuf			 */
	double		elev;		/* elevation			 */
	double          mu0;		/* cosine solar zenith		 */
	double          R0;		/* mean surface albedo		 */
	double          tau;		/* optical depth at zelev	 */
	double          btrans;		/* beam transmittance		 */
	double          gamma[4];	/* 2-stream gamma values	 */
	double          press;		/* reference pressure		 */
	double          refl;		/* reflectance			 */
	double          trans;		/* total transmittance		 */
	double		op_dep;		/* optical depth		 */
	double          zpress;		/* pressure			 */
	double		bval[2];	/* min/max beam radiation	 */
	double		dval[2];	/* min/max diffuse radiation	 */


	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);

   /* open temporary output file */

	tempfile = mktemplate("gelevrad");
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

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}
	nbytes = nsamps * OBANDS * sizeof(fpixel_t);

   /* reference pressure (at reference elevation, in km) */

	press = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
		       zelev * 1.e-3, GRAVITY, MOL_AIR);

   /* initialize bval/dval */
   /* bval is min/max of beam; dval is min/max of diffuse */

	bval[0] = dval[0] = S0;
	bval[1] = dval[1] = 0;


   /* loop reading/writing lines of data, performing calculations on each sample */

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

			if (fdm == ERROR || *mbufp) {

				elev = *ibufp++;
				mu0 = *ibufp++;
				R0 = *ibufp++;
				tau = *ibufp++;

				if (mu0 > 0) {

   					/* Convert elevation to pressure, then to
    				 	 * optical depth. */

					zpress = HYSTAT(SEA_LEVEL, STD_AIRTMP,
							STD_LAPSE, elev * 1.e-3,
							GRAVITY, MOL_AIR);
					op_dep = tau * zpress / press;
	
					/* compute gamma vals (same for all tau) */
	
					(void) mwgamma (mu0, omega, gfact, gamma,
							1);

					/* two stream solution for beam & diffuse*/

					if (twostream (gamma, omega, mu0,
			      			(double) op_dep, R0,
			      			&refl, &trans, &btrans) == ERROR)
					   	error ("twostream error");

					*obufp = S0 * btrans;
					bval[0] = MIN (bval[0], *obufp);
					bval[1] = MAX (bval[1], *obufp);
					obufp++;

					*obufp = S0 * mu0 * (trans - btrans);
					dval[0] = MIN (dval[0], *obufp);
					dval[1] = MAX (dval[1], *obufp);
					obufp++;

				} else {	/* sun down */

					*obufp++ = NO_DATA;
					*obufp++ = NO_DATA;
				}

			} else {
				*obufp++ = NO_DATA;
				*obufp++ = NO_DATA;
				ibufp += IBANDS;
			}

			mbufp++;
		}

		/* write output buffer to temporary file */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("error writing output buffer to temp file, line %d",
				 line);
		}
	}

   /* close temporary file */

	(void) uclose (fdt);

	free (ibuf);
	free (obuf);

   /* create new LQH header from data mins/maxs */

	newlqh (fdo, bval, dval);

   /* copy temporary file to output image */

	output (tempfile, fdo);
}
