/*
** NAME
**	mkprecip -- make precip info from input file
**
*/

#include "ipw.h"
#include "pgm.h"

void
mkprecip(void)
{
	FPIXEL	*i_linebuf;	/* buffer for input image		 */
	FPIXEL	*o_linebuf;	/* buffer for output image		 */
	int	 line;		/* loop counter for lines		 */
	int	 samp;		/* loop counter for samples		 */
	FPIXEL	*i_sample;	/* -> to current input sample		 */
	FPIXEL	*o_sample;	/* -> to current output sample		 */

	float	 mass;		/* precip mass (kg/m^2)			 */
	float	 temp;		/* precip temperature (C)		 */
	float	 percent_snow;	/* % of precip mass that's snow (0 - 1.0)*/
	float	 rho_snow;	/* density of snow (kg/m^3)		 */

	/* allocate buffers for images */
	i_linebuf = new_fpbuf(num_samples, IBANDS);
	if (i_linebuf == NULL)
		error("cannot allocate line buffer for input image");
	o_linebuf = new_fpbuf(num_samples, OBANDS);
	if (o_linebuf == NULL)
		error("cannot allocate line buffer for output image");

	for (line = 0; line < num_lines; line++) {
		if (! read_fpbuf(i_image, i_linebuf, num_samples))
			error("cannot read line %d from input image", line + 1);

		for (samp = 0, i_sample = i_linebuf, o_sample = o_linebuf;
		     samp < num_samples; samp++) {
			mass = *i_sample++;
			temp = *i_sample++;

			calc_snow(temp, &percent_snow, &rho_snow);

			*o_sample++ = mass;
			*o_sample++ = percent_snow;
			*o_sample++ = rho_snow;
			*o_sample++ = temp;
		}

		/* output new line */
		if (! write_fpbuf(o_image, o_linebuf, num_samples))
			error("cannot write line %d to output image", line + 1);
	}
}
