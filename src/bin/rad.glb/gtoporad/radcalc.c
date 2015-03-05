/*
** NAME
** 	radcalc -- calculate radiation values for single input line
** 
** SYNOPSIS
**	radcalc (in_buf, n, net, out_buf)
** 	fpixel_t *in_buf;
**	int n;
**	bool_t net;
**	fpixel_t *out_buf;
** 
** DESCRIPTION
** 	radcalc calculates radiation values for the given input line buffer and
**	writes them to the given output line buffer.
** 
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

int
radcalc (
	fpixel_t       *in_buf,		/* -> input buffer		 */
	int             n,		/* number of pixels		 */
	bool_t          net,		/* ? net radiation		 */
	fpixel_t       *out_buf)	/* -> output buffer		 */
{
	fpixel_t	mu0;		/* cosine solar zenith angle	 */
	fpixel_t        brad;		/* beam radiation		 */
	fpixel_t        rad;		/* total radiation		 */
	fpixel_t        drad;		/* diffuse radiation		 */


   /* check for errors in input */

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

	while (--n >= 0) {

		/* mu0 in band MU0_BAND */

		mu0 = in_buf[MU0_BAND];

		if (mu0 > 0) {

			/* beam radiation in band BEAM_BAND */

			brad = rad = in_buf[BEAM_BAND];

			/* multiply by cosine of local illumination angle, */
			/* which is in MU_BAND */

			rad *= in_buf[MU_BAND];

			/* diffuse radiation in DIFFUSE_BAND */

			drad = in_buf[DIFFUSE_BAND];

			/* add diffuse radiation, accounting for sky view factor */

			rad += drad * in_buf[VF_BAND];

			/* add reflection from adjacent terrain */

			rad += in_buf[CT_BAND] * in_buf[R0_BAND] *
				(drad * (1 - in_buf[VF_BAND]) + brad * mu0);

			/* net radiation, if desired */

			if (net) {
				rad *= 1 - in_buf[ALB_BAND];
			}

		} else { /* sun down - no radiation */

			rad = 0.0;
		}

		/* increment pointers for next pixel */

		*out_buf++ = rad;
		in_buf += NBANDS;
	}
	return (OK);
}
