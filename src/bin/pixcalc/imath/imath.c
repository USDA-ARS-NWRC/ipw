/*
** NAME
**	imath -- calculate arithmetic expression for each input sample
**
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	imath(void)
**
** DESCRIPTION
**	This routine calculates an arithmetic expression for each
**	sample in the input image that is not masked.  The result
**	of the expression is the corresponding single-pixel sample
**	sample in the output image.
*/

#include "ipw.h"
#include "pgm.h"
#include "compile.h"

void
imath(void)
{
	FPIXEL	*i_linebuf;	/* buffer for input image		 */
	PIXEL	*m_linebuf;	/* buffer for mask image		 */
	FPIXEL	*o_linebuf;	/* buffer for output image		 */
	int	 line;		/* loop counter for lines		 */
	int	 samp;		/* loop counter for samples		 */
	FPIXEL	*i_sample;	/* -> to current input sample		 */
	PIXEL	*m_sample;	/* -> to current mask sample		 */
	FPIXEL	*o_sample;	/* -> to current output sample		 */
	FPIXEL	 constValue;	/* value of constant expression		 */


	/*
	 *  Allocate buffers for images
	 */
	if (i_image != NULL) {
		i_linebuf = new_fpbuf(num_samples, num_bands);
		if (i_linebuf == NULL)
			error("cannot allocate line buffer for input image");
	}
	o_linebuf = new_fpbuf(num_samples, 1);
	if (o_linebuf == NULL)
		error("cannot allocate line buffer for output image");
	if (mask != NULL) {
		m_linebuf = new_pbuf(num_samples, constantExpr ? 1 : num_bands);
		if (m_linebuf == NULL)
			error("cannot allocate line buffer for mask image");
	} else {
		m_linebuf = NULL;
	}

	/*
	 *  If we have a constant expression, then execute it once.
	 *  Fill the output buffer with the result, and write out the
	 *  the buffer for each output line.
	 */
	if (constantExpr) {
		if (! execute(cmd_buf, num_cmds, NULL, 0, &constValue))
			error("at line %d, sample %d:", line, samp);
		if (mask == NULL) {
			o_sample = o_linebuf;
			for (samp = 0; samp < num_samples; samp++)
				*o_sample++ = constValue;
			for (line = 0; line < num_lines; line++)
				if (! write_fpbuf(o_image, o_linebuf,
						  num_samples))
					error("cannot write line %d to output"
					      " image", line);
		} else {
			for (line = 0; line < num_lines; line++) {
				if (! read_pbuf(mask, m_linebuf, num_samples))
					error("cannot read line %d from mask"
					      " image", line);
				m_sample = m_linebuf;
				o_sample = o_linebuf;
				for (samp = 0; samp < num_samples; samp++)
					*o_sample++ = *m_sample++ ? constValue
								  : 0;
				if (! write_fpbuf(o_image, o_linebuf,
						  num_samples))
					error("cannot write line %d to output"
					      " image", line);
			}
		}
		return;
	}  /* if constant expression */

	/*
	 *  Otherwise, loop through lines of input image
	 */
	for (line = 0; line < num_lines; line++) {
		if (! read_fpbuf(i_image, i_linebuf, num_samples))
			error("cannot read line %d from input image", line);
		if (mask != NULL) {
			if (! read_pbuf(mask, m_linebuf, num_samples))
				error("cannot read line %d from mask image",
				      line);
		}

		/*
		 *  Loop through samples of current line
		 */
		for (samp = 0, i_sample = i_linebuf, m_sample = m_linebuf,
		     o_sample = o_linebuf;   samp < num_samples;   samp++) {
			if ((mask == NULL) || *m_sample++) {
				/*
				 *  Execute interpreter commands for current
				 *  sample.
				 */
				if (! execute(cmd_buf, num_cmds, i_sample,
					      num_bands, o_sample)) {
					error("at line %d, sample %d:",
					      line, samp);
				}
			} else {
				*o_sample = 0.0;
			}
			i_sample += num_bands;
			o_sample++;
		}  /* for */

		/*
		 *  Output new line
		 */
		if (! write_fpbuf(o_image, o_linebuf, num_samples))
			error("cannot write line %d to output image", line);
	}
}
