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
		bool_t	 first_step,		/* is this the first timestep?	   */
		bool_t	 sun_up[])		/* is S_n band in input image? (2) */
{
	bool_t		masked;		/* mask	flag in temp file	 */

	/* extract bands from input buffers */

	input_rec1.I_lw = *ibuf1_p++;
	input_rec1.T_a  = *ibuf1_p++ + FREEZE;
	input_rec1.e_a  = *ibuf1_p++;
	input_rec1.u    = *ibuf1_p++;
	input_rec1.T_g  = *ibuf1_p++ + FREEZE;
	if (sun_up[0]) 
		input_rec1.S_n = *ibuf1_p++;
	else
		input_rec1.S_n = 0.0;

	input_rec2.I_lw = *ibuf2_p++;
	input_rec2.T_a  = *ibuf2_p++ + FREEZE;
	input_rec2.e_a  = *ibuf2_p++;
	input_rec2.u    = *ibuf2_p++;
	input_rec2.T_g  = *ibuf2_p++ + FREEZE;
	if (sun_up[1]) 
		input_rec2.S_n = *ibuf2_p++;
	else
		input_rec2.S_n = 0.0;

//	printf("sun0 %G - ", input_rec1.S_n);
//	printf("sun1 %G \n", input_rec2.S_n);

	/* if precip event, extract bands from precip buffer */

	if (fdp != ERROR) {
		m_pp         = *pbuf_p++;
		percent_snow = *pbuf_p++;
		rho_snow     = *pbuf_p++;
		T_pp         = *pbuf_p++ + FREEZE;
	}

	/* if first step, set initial or restart conditions  */
	/* otherwise initialize from temporary buffer from last time step */

	if (first_step) {

		/* check if this pixel is masked */

		if ((fdm != ERROR) && (*mbuf_p++ == 0)) {
			/* skip sample in initial-conditions buffer */
			icbuf_p += (restart) ? ICBANDS_RESTART : ICBANDS;
			return FALSE;
		}

		/* extract bands from buffer */

		if (restart) {
			elevation = *icbuf_p++;
			z_0		  = *icbuf_p++;
			z_s       = *icbuf_p++;
			rho	   	  = *icbuf_p++;
			T_s_0	  = *icbuf_p++;
			T_s_l	  = *icbuf_p++;
			T_s	   	  = *icbuf_p++;
			h2o_sat	  = *icbuf_p++;

		} else {	/* init */
			elevation = *icbuf_p++;
			z_0	      = *icbuf_p++;
			z_s       = *icbuf_p++;
			rho	      = *icbuf_p++;
			T_s_0	  = *icbuf_p++;
			T_s	      = *icbuf_p++;
			h2o_sat	  = *icbuf_p++;

			T_s_l = T_s;
		}

		check_range(z_0,      0.0,   50.0, "z_0", PRINT_LINE_SAMP);
		check_range(z_s,      0.0,   50.0, "z_s", PRINT_LINE_SAMP);
		check_range(rho,      0.0, 1000.0, "rho", PRINT_LINE_SAMP);

		/*
		 *  Only check snow temperatures when there's snow.
		 *  Check lower-layer's temperature AFTER initializing the
		 *  snowcover because we need to know how many layers first.
		 */
		if (z_s > 0.0) {
			check_range(T_s_0,  MIN_SNOW_TEMP, 0, "T_s_0",
					PRINT_LINE_SAMP);
			check_range(T_s,    MIN_SNOW_TEMP, 0, "T_s",
					PRINT_LINE_SAMP);
			T_s_0 += FREEZE;
			T_s   += FREEZE;
		}
		check_range(h2o_sat,  0.0,    1.0, "h2o_sat", PRINT_LINE_SAMP);

		/* establish conditions for snowpack */

		T_s_l += FREEZE;
		init_snow();

		/*
		 *  If more than 1 layer, check the range on lower layer's
		 *  temperature.
		 */
		if (layer_count > 1) {
			check_range(T_s_l - FREEZE, MIN_SNOW_TEMP, 0, "T_s_l",
					PRINT_LINE_SAMP);
		}

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

		masked = *itbuf_p++;
		if (masked)
			return FALSE;

		elevation    = *itbuf_p++;
		z_0	     	 = *itbuf_p++;
		z_s          = *itbuf_p++;
		rho	     	 = *itbuf_p++;
		T_s_0	     = *itbuf_p++;
		T_s_l	     = *itbuf_p++;
		T_s	         = *itbuf_p++;
		h2o_sat	     = *itbuf_p++;

		R_n_bar	     = *itbuf_p++;
		H_bar	     = *itbuf_p++;
		L_v_E_bar    = *itbuf_p++;
		G_bar	     = *itbuf_p++;
		M_bar	     = *itbuf_p++;
		delta_Q_bar  = *itbuf_p++;
		E_s_sum      = *itbuf_p++;
		melt_sum     = *itbuf_p++;
		ro_pred_sum  = *itbuf_p++;

		/* establish conditions for snowpack */

		init_snow();
	}

	/* set air pressure from site elev */

	P_a = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE, (elevation / 1000.0),
			GRAVITY, MOL_AIR);

	return TRUE;
}
