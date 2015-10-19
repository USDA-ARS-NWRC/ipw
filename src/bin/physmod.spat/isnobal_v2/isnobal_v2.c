/*
 ** NAME
 ** 	isnobal -- run snowmelt model over a set of images
 **
 ** SYNOPSIS
 **	#include "pgm.h"
 **
 **	void
 **	isnobal(
 **	   int		out_step)	|* # of data tsteps per output img   *|
 **
 ** DESCRIPTION
 ** 	isnobal reads all input files, performs calculations and writes output
 **	images for 2-layer energy-balanced snomelt model.
 **
 ** RETURN VALUE
 **
 ** GLOBALS ACCESSED
 **
 */

#include <math.h>
#include <string.h>

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"
#include "snobal.h"

void
isnobal_v2(
		int		out_step,		/* # of data tsteps per output img   */
		int		got_opt_F)		/* got option F?		     */
{
	double	data_tstep;			/* data timestep		     */
	double	step_time;			/* start time of current data tstep  */
	int	step;					/* step loop counter		     */
	int	end_step;				/* end step index		     */
	bool_t	first_step;			/* is first step? 		     */
	bool_t	last_step;			/* is last step? 		     */
	int	out_counter;			/* counter for data tsteps per output*/
	bool_t	output;				/* output images this step? 	     */
	char	emfile[255];		/* name for temp e/m output file     */
	char	snowfile[255];		/* name for temp snow output file    */
	int	tempin;					/* index of temp filename for input  */
	int	tempout;				/* index of temp filename for output */
	double	timeSinceOut;		/* local copy of 'time_since_out'    */
	bool_t	sun_up[2];			/* is S_n band in input images?      */
	char	pre_img[255];		/* file name for precip image	     */
	int N;						/* number of grid point */
	int n;						/* loop iteration */


	data_tstep = tstep_info[DATA_TSTEP].time_step;

	nsamps = hnsamps(fdic);
	nlines = hnlines(fdic);
	N = nsamps*nlines;

	first_em_pix = TRUE;
	first_snow_pix = TRUE;

	tempout = 0;

	/*
	 * Since no output required by 'snobal' library, don't give it an output
	 * function.
	 */
	out_func = NULL;

	/* Allocate I/O buffers for image files */

	buffers();

	/* open temporary files for output images */

	temp_filename("isnobal", emfile);
	temp_filename("isnobal", snowfile);


	/* initialize time of starting step */

	step_time = start_step * data_tstep;

	/* determine # of digits for the suffixes of input and output images */

	end_step = start_step + nstep - 1;
	nDigits = (int) (log10((double) end_step)) + 1;

	/* loop on data timesteps */

	out_counter = 0;
	timeSinceOut = 0.0;

	for (step = start_step; step <= end_step; step++) {

		first_step = (step == start_step);
		last_step =  (step == end_step);

		/* open input image files */

		if (first_step) {
			fdi1 = open_input(in_prefix, step,   &sun_up[0]);
		} else {
			sun_up[0] = sun_up[1];
		}
		fdi2 = open_input(in_prefix, step+1, &sun_up[1]);

		/* check for precip data this time step */

		if (pfp != NULL) {
			if (precip_event(SEC_TO_HR(step_time), pre_img)) {
				fdp = uropen(pre_img);
				if (fdp == ERROR) {
					error("can't open image \"%s\"",
							pre_img);
				}
				precip_hdrs(pre_img);
			} else {
				fdp = ERROR;
			}
		} else {
			fdp = ERROR;
		}

		/*
		 *  Do we write output images this time step?  If it's
		 *  the last step, then we do.
		 *  If writing output images, open temp files for pixel data.
		 */

		out_counter++;
		if (last_step) {
			output = TRUE;
		} else if (out_counter == out_step) {
			output = TRUE;
			out_counter = 0;
		} else {
			output = FALSE;
		}

		if (output) {
			fdem = uwopen(emfile);
			if (fdem == ERROR) {
				error("Can't open temporary file '%s' for output",
						emfile);
			}
			fds = uwopen(snowfile);
			if (fds == ERROR) {
				error("Can't open temporary file '%s' for output",
						snowfile);
			}
		}

		/* read input data and do calculations */

		read_data(first_step);

		/* reset pointers for output buffers */

		embuf_p = embuf;
		sbuf_p  = sbuf;
		otbuf_p = otbuf;
		ot_nbytes = 0;

		for (n = 0; n < N; n++) {

			/* initialize some global variables for
			   'snobal' library for each pass since
			   the routine 'do_data_tstep' modifies them */

			current_time = step_time;
			time_since_out = timeSinceOut;

			precip_now = (fdp != ERROR);

			/* extract data from I/O buffers */

			if (extract_data(first_step, sun_up)) {

				/* run model on data for this pixel */

				if (! do_data_tstep())
					error("During step %d, at line %d, sample %d",
							step, line, samp);

				/* assign data to output buffers */

				assign_buffers(FALSE, output);

			} else { /* masked point */
				assign_buffers(TRUE, output);
			}

		}  /* for loop on grid */

		/* write output buffers to output files */

		write_data(output, last_step);

		/* close input images */
		if (first_step) {
			fpclose(fdi1);
		}
		fpclose(fdi2);

		if (fdp != ERROR) {
			fpclose(fdp);
		}

		/* increment time */

		step_time += data_tstep;
		if (output) {
			timeSinceOut = 0.0;
		}
		else {
			timeSinceOut += data_tstep;
		}

		/* output any images for this time step */

		if (output) {
			uclose(fdem);
			e_m_image(step, emfile);
			first_em_pix = TRUE;

			uclose(fds);
			snow_image(step, snowfile);
			first_snow_pix = TRUE;
		}

	}  /* for loop on timesteps */

	/* remove temp files */

	uremove(emfile);
	uremove(snowfile);

}
