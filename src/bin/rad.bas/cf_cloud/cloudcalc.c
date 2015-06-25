/*
 **	Calculate cloud attenuated radiation values for this input line.
 **	(See cloud.c for the order of the values in the input line.)
 */

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

int
cloudcalc(
		fpixel_t       *in_buf,		/* -> input buffer		 */
		int             n,		/* number of pixels		 */
		fpixel_t       *out_buf,	/* -> output buffer		 */
		int		obands)		/* # of output bands = 3	 */
{
	fpixel_t        grad;		/* total radiation		 */
	fpixel_t        c_grad;		/* cloud corrected grad		 */
	fpixel_t        drad;		/* diffuse radiation		 */
	fpixel_t        c_drad;		/* cloud corrected drad		 */
	fpixel_t        cf;		/* cloud attenuation factor	 */
	float		brad;		/* beam radiation		 */
	float		c_brad;		/* cloud corrected brad		 */
	float		df_0;		/* initial diffuse rad ratio	 */
	float		bf_0;		/* initial beam rad ratio		 */
	float		bf_c;		/* cloudy beam rad ratio		 */

	/*
	 * check for errors in input
	 */
	if (in_buf == NULL) {
		usrerr("cloudcalc: input buffer NULL");
		return (ERROR);
	}
	if (out_buf == NULL) {
		usrerr("cloudcalc: output buffer NULL");
		return (ERROR);
	}
	if (n <= 0) {
		usrerr("cloudcalc: length of buffers = %d", n);
		return (ERROR);
	}

	while (--n >= 0) {
		/*
		 * global radiation in band GLOBAL_BAND
		 */
		grad = in_buf[GLOBAL_BAND];

		/*
		 * diffuse radiation in DIFFUSE_BAND
		 */
		drad = in_buf[DIFFUSE_BAND];

		/*
		 * cloud factor in CLOUD_BAND
		 */
		cf = in_buf[CLOUD_BAND];

		/*
		 * extensive cloud attenuation, no beam
		 */
		if (cf <= CRAT1) {
			c_grad = grad * cf;
			c_brad = 0.0;
			c_drad = c_grad;

			/*
			 * minimal cloud attenuation, no beam ratio reduction
			 */		} else if (cf > CRAT2) {
				 c_grad = grad * cf;
				 c_drad = drad * cf;
				 c_brad = c_grad - c_drad;


				 /*
				  * cloud attenuation, beam ratio is reduced
				  */
			 } else {
				 bf_c = CCOEF * ((cf - CRAT1) * (cf - CRAT1));
				 c_grad = grad * cf;
				 c_brad = c_grad * bf_c;
				 c_drad = c_grad - c_brad;
			 }
		/*
		 * increment pointers for next pixel
		 */
		*out_buf++ = c_grad;
		*out_buf++ = c_drad;
		in_buf += NBANDS;
	}
	return (OK);
}
