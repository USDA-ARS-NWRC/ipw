/*
 ** NAME
 ** 	extract_data -- extract all input data for single pixel from I/O buffers
 **
 ** SYNOPSIS
 **	bool_t
 **	extract_data(
 **	    bool_t	 first_step,	|* is this the first timestep?	   *|
 **	    bool_t	 sun_up[])	|* is S_n band in input image? (2) *|
 **
 ** DESCRIPTION
 ** 	extract_data extracts all input data from the input, mask, precip
 **	and initial conditions or temporary file for a single pixel.
 **
 ** RETURN VALUE
 **	TRUE	Data was extracted for the pixel.
 **
 **	FALSE	The pixel is masked.
 **
 ** GLOBALS VARIABLES READ
 **
 ** GLOBALS VARIABLES MODIFIED
 **	ibuf1_p
 **	ibuf2_p
 **	icbuf2_p
 **	itbuf_p
 **	mbuf_p
 **	pbuf_p
 **
 **	cc_s_0
 **	cc_s_l
 **	delta_Q_bar
 **	E_s_sum
 ** 	elevation
 **	G_bar
 **	H_bar
 **	h2o
 **	h2o_sat
 ** 	input_rec1
 ** 	input_rec2
 **	L_v_E_bar
 **	M_bar
 **	m_pp
 **	m_s
 **	melt_sum
 **	percent_snow
 **	R_n_bar
 **	rho
 **	rho_snow
 **	ro_pred_sum
 **	T_pp
 **	T_s
 **	T_s_0
 **	T_s_l
 **	z_0
 **	z_s
 **	z_s_l
 */

#include "ipw.h"
#include "pgm.h"
#include "snobal.h"
#include "envphys.h"


/*
 *  This is just to make the calls to 'check_range' more readable.
 *  The last parameter is a flag that controls whether the current
 *  line # and sample # are printed in an error message.
 */
#define PRINT_LINE_SAMP TRUE

bool_t
extract_data(
		bool_t first_step,		/* is this the first timestep? */
		int n, 					/* grid pixel to extract */
		bool_t sun_up[],		/* is S_n band in input image? (2) */
		OUTPUT_REC *output_rec[])		/* ouput sturcture */
{
	bool_t masked;		/* mask	flag in temp file	 */
	bool_t rt = TRUE;	/* return value */
	int ibuf_start;		/* start index for the input buffer */
	int pbuf_start;		/* start index for the precip buffer */
	int icbuf_start;	/* start index for the initial conditions buffer */


	/* extract bands from input buffers */
	if (sun_up[0])
		ibuf_start = n * IBANDS;
	else
		ibuf_start = n * (IBANDS - 1);

	input_rec1.I_lw = ibuf1[ibuf_start++];
	input_rec1.T_a  = ibuf1[ibuf_start++] + FREEZE;
	input_rec1.e_a  = ibuf1[ibuf_start++];
	input_rec1.u    = ibuf1[ibuf_start++];
	input_rec1.T_g  = ibuf1[ibuf_start++] + FREEZE;
	if (sun_up[0]) 
		input_rec1.S_n = ibuf1[ibuf_start++];
	else
		input_rec1.S_n = 0.0;

	if (sun_up[1])
		ibuf_start = n * IBANDS;
	else
		ibuf_start = n * (IBANDS - 1);

	input_rec2.I_lw = ibuf2[ibuf_start++];
	input_rec2.T_a  = ibuf2[ibuf_start++] + FREEZE;
	input_rec2.e_a  = ibuf2[ibuf_start++];
	input_rec2.u    = ibuf2[ibuf_start++];
	input_rec2.T_g  = ibuf2[ibuf_start++] + FREEZE;
	if (sun_up[1]) 
		input_rec2.S_n = ibuf2[ibuf_start++];
	else
		input_rec2.S_n = 0.0;


	/*
	 * Check the input value ranges
	 */
	check_range (n, input_rec1.S_n, 0.0, 800.0, "S_n", PRINT_LINE_SAMP);
	check_range (n, input_rec1.I_lw, 0.0, 600.0, "I_lw", PRINT_LINE_SAMP);
	check_range (n, input_rec1.T_a, 200.0, 320.0, "T_a", PRINT_LINE_SAMP);
	check_range (n, input_rec1.e_a, 0.0, 5000.0, "e_a", PRINT_LINE_SAMP);
	check_range (n, input_rec1.u, 0.0, 500.0, "u", PRINT_LINE_SAMP);
	check_range (n, input_rec1.T_g, 180.0, 300.0, "T_g", PRINT_LINE_SAMP);

	//	check_range (input_rec2.S_n, 0.0, 800.0, "S_n", PRINT_LINE_SAMP);
	//	check_range (input_rec2.I_lw, 0.0, 600.0, "I_lw", PRINT_LINE_SAMP);
	//	check_range (input_rec2.T_a, 200.0, 320.0, "T_a", PRINT_LINE_SAMP);
	//	check_range (input_rec2.e_a, 0.0, 2000.0, "e_a", PRINT_LINE_SAMP);
	//	check_range (input_rec2.u, 0.0, 500.0, "u", PRINT_LINE_SAMP);
	//	check_range (input_rec2.T_g, 180.0, 300.0, "T_g", PRINT_LINE_SAMP);


	/* if precip event, extract bands from precip buffer */

	if (fdp != ERROR) {

		pbuf_start = n * PBANDS;

		m_pp         = pbuf[pbuf_start++];
		percent_snow = pbuf[pbuf_start++];
		rho_snow     = pbuf[pbuf_start++];
		T_pp         = pbuf[pbuf_start] + FREEZE;

	}

	/* if first step, set initial or restart conditions  */
	/* otherwise initialize from temporary buffer from last time step */

	if (first_step) {

		/* check if this pixel is masked */

		if ((fdm != ERROR) && (mbuf[n] == 0)) {
			/* skip sample in initial-conditions buffer */
			// icbuf_p += (restart) ? ICBANDS_RESTART : ICBANDS;
			return FALSE;
		}

		if (restart)
			icbuf_start = n * ICBANDS_RESTART;
		else
			icbuf_start = n * ICBANDS;

		/* extract bands from buffer */

		if (restart) {
			elevation = icbuf[icbuf_start++];
			z_0		  = icbuf[icbuf_start++];
			z_s       = icbuf[icbuf_start++];
			rho	   	  = icbuf[icbuf_start++];
			T_s_0	  = icbuf[icbuf_start++];
			T_s_l	  = icbuf[icbuf_start++];
			T_s	   	  = icbuf[icbuf_start++];
			h2o_sat	  = icbuf[icbuf_start++];

		} else {	/* init */
			elevation = icbuf[icbuf_start++];
			z_0	      = icbuf[icbuf_start++];
			z_s       = icbuf[icbuf_start++];
			rho	      = icbuf[icbuf_start++];
			T_s_0	  = icbuf[icbuf_start++];
			T_s	      = icbuf[icbuf_start++];
			h2o_sat	  = icbuf[icbuf_start++];

			T_s_l = T_s;
		}

		check_range(n, z_0,      0.0,   50.0, "z_0", PRINT_LINE_SAMP);

		/* establish conditions for snowpack */
		if (T_s_0 < MIN_SNOW_TEMP) {
			T_s_0 = MIN_SNOW_TEMP;
		}
		if (T_s < MIN_SNOW_TEMP) {
			T_s = MIN_SNOW_TEMP;
		}
		if (T_s_l < MIN_SNOW_TEMP) {
			T_s_l = MIN_SNOW_TEMP;
		}
		T_s_0 += FREEZE;
		T_s   += FREEZE;
		T_s_l += FREEZE;
		init_snow();

		/* initialize totals */

		R_n_bar	     = 0.0;
		H_bar	     = 0.0;
		L_v_E_bar    = 0.0;
		G_bar	     = 0.0;
		M_bar	     = 0.0;
		delta_Q_bar  = 0.0;
		E_s_sum      = 0.0;
		melt_sum     = 0.0;
		ro_pred_sum  = 0.0;
	}
	else {

		/* get last time step results from temp */

		/* check to see if point is masked */
		masked = output_rec[n]->masked;
		if (masked) {
			rt = FALSE;

		} else {

			elevation    = output_rec[n]->elevation;
			z_0	     	 = output_rec[n]->z_0;
			z_s          = output_rec[n]->z_s;
			rho	     	 = output_rec[n]->rho;
			T_s_0	     = output_rec[n]->T_s_0;
			T_s_l	     = output_rec[n]->T_s_l;
			T_s	         = output_rec[n]->T_s;
			h2o_sat	     = output_rec[n]->h2o_sat;
			layer_count  = output_rec[n]->layer_count;

			R_n_bar	     = output_rec[n]->R_n_bar;
			H_bar	     = output_rec[n]->H_bar;
			L_v_E_bar    = output_rec[n]->L_v_E_bar;
			G_bar	     = output_rec[n]->G_bar;
			M_bar	     = output_rec[n]->M_bar;
			delta_Q_bar  = output_rec[n]->delta_Q_bar;
			E_s_sum      = output_rec[n]->E_s_sum;
			melt_sum     = output_rec[n]->melt_sum;
			ro_pred_sum  = output_rec[n]->ro_pred_sum;

			/* establish conditions for snowpack */

			init_snow();

			//	printf(";Ts %f Ta %f;", T_s_0, input_rec2.T_a);

		}

	}

	if (first_step | !masked) {
		warn_range(n, z_s, 0.0, 50.0, "z_s", PRINT_LINE_SAMP);
		warn_range(n, rho, 0.0, 1000.0, "rho", PRINT_LINE_SAMP);

		/*
		 *  Only check snow temperatures when there's snow.
		 *  Check lower-layer's temperature AFTER initializing the
		 *  snowcover because we need to know how many layers first.
		 */
		if (z_s > 0.0) {
			warn_range(n, T_s_0 - FREEZE, MIN_SNOW_TEMP, 0, "T_s_0", PRINT_LINE_SAMP);
			warn_range(n, T_s - FREEZE, MIN_SNOW_TEMP, 0, "T_s", PRINT_LINE_SAMP);
		}
		warn_range(n, h2o_sat, 0.0, 1.0, "h2o_sat", PRINT_LINE_SAMP);

		/*
		 *  If more than 1 layer, check the range on lower layer's
		 *  temperature.
		 */
		if (layer_count > 1) {
			warn_range(n, T_s_l - FREEZE, MIN_SNOW_TEMP, 0, "T_s_l", PRINT_LINE_SAMP);
		}
	}

	/* set air pressure from site elev */

	P_a = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE, (elevation / 1000.0),
			GRAVITY, MOL_AIR);

	return rt;

}
