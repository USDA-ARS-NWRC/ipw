/*
 **	Calculate adjusted thermal radiation values for this input line.
 **	(See th_adj.c for the order of the values in the input line.)
 */

#include "ipw.h"
#include "fpio.h"
#include "envphys.h"
#include "radiation.h"
#include "pgm.h"

int
adjust(
		fpixel_t       *in_buf,		/* -> input buffer		 */
		int             n,		/* number of pixels		 */
		fpixel_t       *out_buf,	/* -> output buffer		 */
		int		obands)		/* # of output bands = 1	 */
{
	fpixel_t        th_clear;	/* clear-sky thermal radiation	 */
	fpixel_t        th_adj;		/* corrected thermal radiation	 */
	fpixel_t        ta;		/* diffuse radiation		 */
	fpixel_t        sky_view;	/* sky-view factor	 	 */
	float		bb_th;		/* black-body thermal radiation	 */

	/*
	 * check for errors in input
	 */
	if (in_buf == NULL) {
		usrerr("adjust: input buffer NULL");
		return (ERROR);
	}
	if (out_buf == NULL) {
		usrerr("adjust: output buffer NULL");
		return (ERROR);
	}
	if (n <= 0) {
		usrerr("adjust: length of buffers = %d", n);
		return (ERROR);
	}

	while (--n >= 0) {
		/*
		 * clear-sky thermal radiation in band THERMALIN_BAND
		 */
		th_clear = in_buf[THERMALIN_BAND];
		/*
		 * air temperature in AIRTEMP_BAND
		 */
		ta = in_buf[AIRTEMP_BAND];
		ta += FREEZE;
		/*
		 * sky-view factor in FACTOR_BAND
		 */
		sky_view = in_buf[FACTOR_BAND];
		if (sky_view > 1.0) {
			usrerr("adjust: sky_view > 1.0 = %g", sky_view);
			return (ERROR);
		}

		/*
		 * set black-body thermal
		 */
		bb_th = STEF_BOLTZ * ta * ta * ta * ta;

		/*
		 * calculate adjusted radiation
		 */
		th_adj = (th_clear * sky_view) + (bb_th * (1.0 - sky_view));

		/*
		 * increment pointers for next pixel
		 */
		*out_buf++ = th_adj;
		in_buf += NBANDS;
	}
	return (OK);
}
