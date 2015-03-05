/*
** NAME
**	interp_image -- interpolate each point of an n-band image over table
**
*/

#include "ipw.h"
#include "pgm.h"

void
interp_image(void)
{
	fpixel_t *i_linebuf;	/* buffer for input image		 */
	fpixel_t *o_linebuf;	/* buffer for output image		 */
	int	  line;		/* loop counter for lines		 */
	int	  samp;		/* loop counter for samples		 */
	fpixel_t *i_sample;	/* -> to current input sample		 */
	fpixel_t *o_sample;	/* -> to current output sample		 */


	/*
	 *  allocate buffers for images
	 */
	i_linebuf = new_fpbuf(num_samples, num_bands);
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
			*o_sample++ = n_interp(num_bands, DimensionInfo, Table,
					       i_sample);
			i_sample += num_bands;
		}

		/* output new line */
		if (! write_fpbuf(o_image, o_linebuf, num_samples))
			error("cannot write line %d to output image", line + 1);
	}
}
