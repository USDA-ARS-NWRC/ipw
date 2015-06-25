/*
 **	Calculate radiation values for this input line.
 **	(See toporad.h for the order of the values in the input line.)
 */

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

int
radcalc(
		fpixel_t       *in_buf,		/* -> input buffer		 */
		int             n,		/* number of pixels		 */
		bool_t          net,		/* ? net radiation		 */
		fpixel_t        mu0,		/* cosine of solar zenith angle	 */
		fpixel_t       *out_buf,	/* -> output buffer		 */
		int		obands,		/* # of output bands = 1 or 2	 */
		bool_t		got_R0,		/* ? R0 provided		 */
		float		R0)		/* reflectance of the substrate	 */
{
	fpixel_t        brad;		/* beam radiation		 */
	fpixel_t        drad;		/* diffuse radiation (from sky)	 */
	fpixel_t        rad;		/* total radiation		 */
	fpixel_t	diffuse;	/* diffuse (includes reflections
						    from terrrain	 */

	/*
	 * check for errors in input
	 */
	if (in_buf == NULL) {
		usrerr("radcalc: input buffer NULL");
		return (ERROR);
	}
	if (out_buf == NULL) {
		usrerr("radcalc: output buffer NULL");
		return (ERROR);
	}
	if (n <= 0) {
		usrerr("radcalc: length of buffers = %d", n);
		return (ERROR);
	}
	if (mu0 < 0 || mu0 > 1) {
		usrerr("cosine of solar angle = %g", mu0);
		return (ERROR);
	}

	while (--n >= 0) {
		/*
		 * beam radiation in band BEAM_BAND
		 */
		brad = in_buf[BEAM_BAND];
		/*
		 * diffuse radiation in DIFFUSE_BAND
		 */
		drad = in_buf[DIFFUSE_BAND];
		/*
		 * adjust diffuse radiation accounting for sky view factor
		 */
		diffuse = drad * in_buf[VF_BAND];
		/*
		 * add reflection from adjacent terrain
		 */
		diffuse += (drad * (1 - in_buf[VF_BAND]) + brad * mu0) *
				in_buf[CT_BAND] * ( got_R0 ? R0
						: in_buf[ALB_BAND]);
		/*
		 * global radiation is diffuse + incoming_beam * cosine of local illumination
		 * angle, which is in MU_BAND
		 */
		rad = diffuse + brad * in_buf[MU_BAND];

		/*
		 * net radiation, if desired
		 */
		if (net) {
			rad *= 1 - in_buf[ALB_BAND];
			diffuse *= 1 - in_buf[ALB_BAND];
		}

		/*
		 * increment pointers for next pixel
		 */
		*out_buf++ = rad;
		if (obands == 2) {
			*out_buf++ = diffuse;
		}
		in_buf += NBANDS;
	}
	return (OK);
}
