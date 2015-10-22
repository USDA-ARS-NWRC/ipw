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
//#include <omp.h>
#include "ipw.h"
#include "fpio.h"
#include "pgm.h"
#include "snobal.h"

void
isnobal_v2(
		int	out_step,		/* # of data tsteps per output img   */
		int nthreads,		/* number of threads to use */
		int	got_opt_F)		/* got option F?		     */
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
	//	int	tempin;					/* index of temp filename for input  */
	//	int	tempout;				/* index of temp filename for output */
	double	timeSinceOut;		/* local copy of 'time_since_out'    */
	bool_t	sun_up[2];			/* is S_n band in input images?      */
	char	pre_img[255];		/* file name for precip image	     */
	int N;						/* number of grid point */
	int n;						/* loop iteration */


	//	printf("here\n");

	data_tstep = tstep_info[DATA_TSTEP].time_step;

	nsamps = hnsamps(fdic);
	nlines = hnlines(fdic);
	N = nsamps*nlines;

	// array of pointers to OUTPUT_REC
	OUTPUT_REC *output_rec[N];
	for(n = 0; n < N; ++n)
		output_rec[n] = (OUTPUT_REC *)malloc(sizeof(OUTPUT_REC));	// initialize the memory at that pointer

	//	first_em_pix = TRUE;
	//	first_snow_pix = TRUE;

	//	tempout = 0;

	/* set threads */
	omp_set_dynamic(0);     		// Explicitly disable dynamic teams
	omp_set_num_threads(nthreads); 	// Use N threads for all consecutive parallel regions

	/*
	 * Since no output required by 'snobal' library, don't give it an output
	 * function.
	 */
	out_func = NULL;

	/* Allocate I/O buffers for image files */

	buffers();

	/* open temporary files for output images */

	//	temp_filename("isnobal", emfile);
	//	temp_filename("isnobal", snowfile);


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

		//		printf("Time step -- %i\n", step);

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

		//		if (output) {
		//			fdem = uwopen(emfile);
		//			if (fdem == ERROR) {
		//				error("Can't open temporary file '%s' for output",
		//						emfile);
		//			}
		//			fds = uwopen(snowfile);
		//			if (fds == ERROR) {
		//				error("Can't open temporary file '%s' for output",
		//						snowfile);
		//			}
		//		}

		/* read input data and do calculations */

		read_data(first_step);

		/* reset pointers for output buffers */

		//		ot_nbytes = 0;


		//				#pragma omp threadprivate(elevation, m_ppm, \
		//					rho_snow, T_pp, layer_count,run_no_snow,stop_no_snow,max_z_s_0,max_h2o_vol,tstep_info, \
		//					time_step,current_time,time_since_out,layer_count,z_s,z_s_0,z_s_l,rho,m_s,m_s_0,m_s_l, \
		//					T_s,T_s_0,T_s_l,cc_s,cc_s_0,cc_s_l,h2o_sat,h2o_vol,h2o,h2o_max,h2o_total,ro_data,input_rec1, \
		//					input_rec2,S_n,I_lw,T_a,e_a,u,T_g,ro,P_a,relative_hts,z_g,z_u,z_T,z_0,precip_now,m_pp, \
		//					percent_snow,rho_snow,T_pp,T_rain,T_snow,h2o_sat_snow,m_precip,m_rain,m_snow,z_snow, \
		//					R_n,H,L_v_E,G,G_0,M,delta_Q,delta_Q_0,R_n_bar,H_bar,L_v_E_bar,G_bar,G_0_bar,M_bar,\
		//					delta_Q_bar,delta_Q_0_bar,melt,E,E_s,ro_predict,melt_sum,E_s_sum,ro_pred_sum)
		//#pragma omp parallel shared(output_rec, fdp, ibuf1, ibuf2, icbuf, pbuf, mbuf, embuf, sbuf, step) private(nsamps,nlines)
		//		,z_u,z_T,z_g,\relative_hts,z_0,T_s_0,T_a)
		//			private(n,nsamps,nlines,elevation,z_0,z_s,rho,T_s_0,T_s,T_s_l,h2o_sat,T_a,T_g,\
		//					R_n_bar, H_bar, L_v_E_bar, G_bar, M_bar, delta_Q_bar, E_s_sum, melt_sum, ro_pred_sum)
		//private(rho_snow, T_pp, layer_count,run_no_snow,stop_no_snow,max_z_s_0,max_h2o_vol, \
		//		time_step,current_time,time_since_out,z_s,z_s_0,z_s_l,rho,m_s,m_s_0,m_s_l, \
		//		T_s,T_s_0,T_s_l,cc_s,cc_s_0,cc_s_l,h2o_sat,h2o_vol,h2o,h2o_max,h2o_total,ro_data,input_rec1, \
		//		input_rec2,S_n,I_lw,T_a,e_a,u,T_g,ro,P_a,z_0,precip_now,m_pp, \
		//		percent_snow,T_rain,T_snow,h2o_sat_snow,m_precip,m_rain,m_snow,z_snow, \
		//		R_n,H,L_v_E,G,G_0,M,delta_Q,delta_Q_0,R_n_bar,H_bar,L_v_E_bar,G_bar,G_0_bar,M_bar,\
		//		delta_Q_bar,delta_Q_0_bar,melt,E,E_s,ro_predict,melt_sum,E_s_sum,ro_pred_sum)


#pragma omp parallel shared(output_rec, ibuf1, ibuf2, icbuf, pbuf, mbuf, sbuf, embuf, fdp, fdm, restart, output, first_step)\
		private(n) \
		copyin(tstep_info, z_u, z_T, z_g, relative_hts, max_z_s_0, max_h2o_vol)
		{

#pragma omp for
			for (n = 0; n < N; n++) {

				/* initialize some global variables for
			   'snobal' library for each pass since
			   the routine 'do_data_tstep' modifies them */

				current_time = step_time;
				time_since_out = timeSinceOut;

				precip_now = (fdp != ERROR);

				/* extract data from I/O buffers */

				if (extract_data(first_step, n, sun_up, output_rec)) {

					/* run model on data for this pixel */

					if (! do_data_tstep())
						error("During step %d, at pixel %i", step, n);

					/* assign data to output buffers */

					assign_buffers(FALSE, n, output, output_rec);

				} else { /* masked point */
					assign_buffers(TRUE, n, output, output_rec);
				}

			}  /* for loop on grid */
		} /* end of parallel region */

		/* write output buffers to output files */

		//		write_data(output, last_step);

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
			e_m_image(step, output_rec);
			//			first_em_pix = TRUE;

			uclose(fds);
			snow_image(step, output_rec);
			//			first_snow_pix = TRUE;
		}

	}  /* for loop on timesteps */

	/* remove temp files */

	uremove(emfile);
	uremove(snowfile);

}
