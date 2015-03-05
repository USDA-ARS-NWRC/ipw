/*
** name
**	iptx -- calculate and write out values of PE
**
** synopsis
**	void iptx (fdi, fdm, fdo)
**	int fdi, fdm, fdo;
**
** description
**      Iptx calculates le flux using the Priestly and Taylor (1972)
**      approach. This method is designed to estimate the total
**      evaporative flux over a wet surface. The le flux is thus
**      representative of potential evaporation (PE). The input image
**      has 3 bands:
**
**		band	contents
**		----    --------
**		0	net radiation (negative away from surface)
**		1	air temperature (C)
**		2	Priestly Taylor Parameter (approximately 1.26)
**
**	Assumptions:
**
**	G is 10% of Rn; Surface is saturated and le due to turbulent
**	vapor flux is minimal and well correlated with Rn. This
**	assumption works well over short canopies (grass) but
**	not over forested canopies (McNaughton and Black 1973;
**	Priestley and Taylor 1972).
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "envphys.h"
#include "pgm.h"
 

#define SATVP_EQN1(temp) (					\
		100*pow(1.e1, -9.09718*((273.15/(temp))-1.) 	\
		- 3.56654*log(273.15/(temp))/log(1.e1)		\
		+ 8.76793e-1 *(1.-((temp)/273.15))		\
		+ log(6.1071)/log(1.e1))			\
	)
 
#define SATVP_EQN2(temp) (						\
	    	-7.90298*(BOIL/(temp)-1.)				\
		+ 5.02808*log(BOIL/(temp))/log(1.e1)			\
		- 1.3816e-7*(pow(1.e1, 1.1344e1*(1.-(temp)/BOIL))-1.)	\
		+ 8.1328e-3*(pow(1.e1,-3.49149*(BOIL/(temp)-1.))-1.)	\
		+ log(SEA_LEVEL)/log(1.e1)				\
	)


void
iptx(
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

        double  	Rn;
        double  	ta;
        double  	PP;
        double  	arad;
        double 		tak;
        double 		t1;
        double 		t2;
        double 		sat_vpres;
        double 		sat_vpres1;
        double 		sat_vpres2;
        double 		Gamma;
        double  	slope;
        double  	delta;
        double  	le;

	double		le_min = 0.0;	/* min le in output image	 */
	double		le_max = 0.0;	/* max le in output image	 */


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

				Rn = vector[0];
				ta = vector[1];
				PP = vector[2];

				/* Calculate available energy (Campbell 1977,
			       	   Running 1992) */
 
    				arad = Rn - (0.1 * Rn);
 
				/* Calculate weighting factor (change in qs
				   with temperature) */

				/* convert to kelvin */
				tak = (ta + 273.15);
				t1 = ((ta + 0.5)+273.15);
				t2 = ((ta - 0.5)+273.15);

				/* calculate saturation vapor pressures in Pa */
 
				if (tak <= 273.15) {
					sat_vpres  = SATVP_EQN1(tak);
					sat_vpres1 = SATVP_EQN1(t1);
					sat_vpres2 = SATVP_EQN1(t2);
        			}
			        else {
					sat_vpres  = SATVP_EQN2(tak);
					sat_vpres1 = SATVP_EQN2(t1);
					sat_vpres2 = SATVP_EQN2(t2);
 
        				sat_vpres = pow(1.e1,sat_vpres);
        				sat_vpres1 = pow(1.e1,sat_vpres1);
        				sat_vpres2 = pow(1.e1,sat_vpres2);
        			}

	                        Gamma = 0.646 + 0.0006 * ta;
       				slope = (sat_vpres1 - sat_vpres2)/100;
				delta = slope / (slope + Gamma);
 
				/* Calculate le (W/m2) */
 
				le = -1.0 * (PP * delta * arad);
 
				*obufp = le;

				/* update min/max */

				if (first_pixel) {
					first_pixel = FALSE;
					le_min = le;
					le_max = le;
				} else {
					le_min = MIN (le_min, le);
					le_max = MAX (le_max, le);
				}

			} else {
				*obufp = NO_DATA;
			}

		ibufp += IBANDS;
		if (mbuf != NULL)
			mbufp++;
		obufp++;
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

	newlqh (fdo, le_min, le_max);

   /* copy temporary file to output image */

	output (tempfile, fdo);
}
