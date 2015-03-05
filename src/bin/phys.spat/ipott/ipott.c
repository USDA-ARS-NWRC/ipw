/*
** NAME
** 	ipott -- calculate sea-level or potential temperature from air
**		 temperature or vice-versa over given image
** 
** SYNOPSIS
**	ipott (fdi, fdm, fd_elev, inv, fdo, lapse, T0, linear)
** 	int fdi, fdm, fd_elev;
**	int inv;
**	int fdo;
**	float lapse;
**	float T0;
**	int linear;
** 
** DESCRIPTION
** 	ipott calculates sea-level or potential temperatures from air
**	temperatures and elevations over the given images, or if inv is set,
**	ipott inverts sea-level or potential temperatures at given elevations
**	for air temperatures.
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "envphys.h"
#include "pgm.h"

void
ipott (
	int		fdi,
	int		fdm,
	int		fd_elev,
	int		inv,
	int 		fdo,
        float           lapse,          /* lapse rate (deg/km)	 	*/
        float           T0,             /* assumed sea-level temp. (C)  */
        int             linear)         /* linear adjustment flag       */
{
	int		nsamps;		/* # samples in images		*/
	int		nlines;		/* # lines in images		*/
	int		line;		/* line loop counter		*/
	int		samp;		/* sample loop counter		*/
	fpixel_t       *i_buf;		/* -> input image line		*/
	pixel_t        *m_buf;		/* -> mask image line		*/
	fpixel_t       *e_buf;		/* -> elevation image line	*/
	fpixel_t       *o_buf;		/* -> output image line		*/
	double		elev;		/* elevation (m)		*/
	double		press;		/* air pressure (Pa)		*/
	double		ta;		/* air temperature (K)		*/
	double		theta;		/* potential temperature (K)	*/

   /* convert numeric options to correct units */

	T0    += FREEZE;	/* to K     */

	nsamps = hnsamps (fdi);
	nlines = hnlines (fdi);

/* fprintf(stderr,"T0 = %g  lapse = %g  linear = %s  inv = %s\n",
 *	T0, lapse, linear ? "true" : "false", inv ? "true":"false");
 */

   /* allocate buffer for input image line */

	i_buf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (i_buf == NULL) {
		error ("Unable to allocate IPW line buffer");
	}

   /* allocate buffer for mask image line */

	if (fdm != ERROR) {
		m_buf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (m_buf == NULL) {
			error ("Unable to allocate mask buffer");
		}
	}

   /* allocate buffer for elev image line */

	e_buf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (e_buf == NULL) {
		error ("Unable to allocate IPW line buffer");
	}

   /* allocate buffer for output image line */

	o_buf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (o_buf == NULL) {
		error ("Unable to allocate IPW line buffer");
	}

   /* loop on image lines, performing calculations */

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (fpvread (fdi, i_buf, nsamps) != nsamps) {
			error ("Error reading input image, line %d", line+1);
		}

		/* read elevation image line */

		if (fpvread (fd_elev, e_buf, nsamps) != nsamps) {
			error ("Error reading elev image, line %d", line+1);
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, m_buf, nsamps) != nsamps) {
				error ("Error reading mask image, line %d", line+1);
			}
		}

		/* convert temp for each sample in line */

		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || m_buf[samp]) {
				elev = e_buf[samp];

			    /* calculate air pressure if necessary */
				
			    if (! linear) {
				press = HYSTAT (SEA_LEVEL, T0, lapse,
				        	(elev/1000.), GRAVITY, MOL_AIR);
			    }

			    /* if "inv" set, calculate air temperature */

			    if (inv) {
			        if (linear) {
					o_buf[samp] = i_buf[samp] + 
					    (lapse * (elev / 1000.));
				} else {
					theta = i_buf[samp] + FREEZE;
					ta = INV_POT_TEMP (theta, press);
					o_buf[samp] = ta - FREEZE;
				}

				/* else, calculate potential temperature */
	
			    } else {
			            if (linear) {
					o_buf[samp] = i_buf[samp] - 
					    (lapse * (elev / 1000.));
/**
fprintf(stderr,"linear: o_buf[samp] = %g  i_buf[samp] = %g  elev = %g \n",
	o_buf[samp], i_buf[samp], elev);
**/

				    } else {
					ta = i_buf[samp] + FREEZE;
					theta = POT_TEMP(ta, press);
					o_buf[samp] = theta - FREEZE;
/**
fprintf(stderr,"ta = %g  theta = %g \n",ta,theta);
**/
				    }
			    }
			} else {
				o_buf[samp] = T_MIN;
			}
		}

		/* write line to output image */

		if (fpvwrite (fdo, o_buf, nsamps) != nsamps) {
			error ("Error writing output image, line %d", line+1);
		}
	}

}
