#include "ipw.h"

#include "pgm.h"

/*
 * accum -- accumulate image sums
 */

void
accum(void)
{
	pixel_t        *c_buf = parm.c_buf;
	int            *c_npixels = parm.c_npixels;
	int             nbands = parm.i_nbands;
	REG_1 fpixel_t *pixel = parm.i_buf;
	int             samp;
	double        **sum_x = parm.sum_x;
	double       ***sum_xy = parm.sum_xy;

	for (samp = 0; samp < parm.nsamps; ++samp) {
		int             band;
		REG_4 int       class = *c_buf++;
		REG_5 double   *c_sum_x;
		REG_6 double  **c_sum_xy;

		++c_npixels[class];

		c_sum_x = sum_x[class];
		c_sum_xy = sum_xy[class];

		for (band = 0; band < nbands; ++band) {
			REG_2 int       band2;
			FREG_1 fpixel_t p_band = pixel[band];
			REG_3 double   *band_c_sum_xy = *c_sum_xy++;

			for (band2 = 0; band2 <= band; ++band2) {
				*band_c_sum_xy++ += p_band * pixel[band2];
			}

			*c_sum_x++ += p_band;
		}

		pixel += nbands;
	}
}
