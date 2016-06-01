/*
 ** NAME
 ** 	assign_buffers -- assign data bands to all output line I/O buffers
 **
 ** SYNOPSIS
 **	#include "pgm.h"
 **
 **	void
 **	assign_buffers (
 **	    int		masked,		|* is point masked?		   *|
 **	    int	        output)		|* writing output image this step? *|
 **
 ** DESCRIPTION
 ** 	assign_buffers assigns data bands to the temporary results buffer and
 **	the energy/mass output and snow output buffers (at appropriate time
 **	steps).
 **
 ** GLOBALS VARIABLES READ
 **	cc_s
 **	delta_Q_bar
 **	E_s_sum
 **	elevation
 **	G_bar
 **	H_bar
 **	h2o_sat
 **	h2o_vol
 **	L_v_E_bar
 **	M_bar
 **	m_s
 **	max_h2o_vol
 **	melt_sum
 **	R_n_bar
 **	rho
 **	ro_pred_sum
 **	T_s
 **	T_s_0
 **	T_s_l
 **	z_0
 **	z_s
 **	z_s_l
 **
 ** GLOBALS VARIABLES MODIFIED
 **	em_mins
 **	em_maxs
 **	embuf_p
 **	first_em_pix
 **	first_snow_pix
 **	ot_nbytes
 **	otbuf_p
 **	s_mins
 **	s_maxs
 **	sbuf_p
 */

#include "ipw.h"
#include "pgm.h"
#include "snobal.h"
#include "envphys.h"

void
assign_buffers (
		int	masked,		/* is point masked?		   */
		int n, 			/* assign to this pixel */
		int	output,		/* writing output image this step? */
		OUTPUT_REC *output_rec[])		/* ouput sturcture */
{
	//	fpixel_t *bufp;		/* -> in line buffers		 */
	int i;				/* band loop counter		 */
	int embuf_start;	/* start index to the em buffer */
	int sbuf_start;	/* start index to the snow buffer */


	/* if output this time step, set output buffer bands */

	if (output) {

		embuf_start = n * EMBANDS;

		if (masked) {
			for (i = 0; i < EMBANDS; i++) {
				embuf[embuf_start++] = NO_DATA;
			}
		} else {
			embuf[embuf_start++] = R_n_bar;
			embuf[embuf_start++] = H_bar;
			embuf[embuf_start++] = L_v_E_bar;
			embuf[embuf_start++] = G_bar;
			embuf[embuf_start++] = M_bar;
			embuf[embuf_start++] = delta_Q_bar;
			embuf[embuf_start++] = E_s_sum;
			embuf[embuf_start++] = melt_sum;
			embuf[embuf_start++] = ro_pred_sum;
			embuf[embuf_start++] = cc_s;

			/* keep min & max of each band */
//#pragma omp critical
//			{
//				embuf_start = n * EMBANDS;
//				if (first_em_pix) {
//					first_em_pix = FALSE;
//					for (i=0; i < EMBANDS; i++) {
//						em_mins[i] = embuf[embuf_start];
//						em_maxs[i] = embuf[embuf_start];
//						embuf_start++;
//					}
//				} else {
//					for (i=0; i < EMBANDS; i++) {
//						em_mins[i] = MIN(embuf[embuf_start], em_mins[i]);
//						em_maxs[i] = MAX(embuf[embuf_start], em_maxs[i]);
//						embuf_start++;
//					}
//				}
//			}
		}

		/* do the same for snow output image */

		sbuf_start = n * SBANDS;

		if (masked) {
			for (i = 0; i < SBANDS; i++) {
				sbuf[sbuf_start++] = NO_DATA;
			}
		} else {

			sbuf[sbuf_start++] = z_s;
			sbuf[sbuf_start++] = rho;
			sbuf[sbuf_start++] = m_s;
			sbuf[sbuf_start++] = h2o;
			sbuf[sbuf_start++] = T_s_0 - FREEZE;
			sbuf[sbuf_start++] = T_s_l - FREEZE;
			sbuf[sbuf_start++] = T_s   - FREEZE;
			sbuf[sbuf_start++] = z_s_l;
			sbuf[sbuf_start++] = h2o_sat;

			/* keep min & max of each band */

//			sbuf_start = n * SBANDS;

//#pragma omp critical
//			{
//				if (first_snow_pix) {
//					first_snow_pix = FALSE;
//					for (i=0; i < SBANDS; i++) {
//						s_mins[i] = sbuf[sbuf_start];
//						s_maxs[i] = sbuf[sbuf_start];
//						sbuf_start++;
//					}
//				} else {
//					for (i=0; i < SBANDS; i++) {
//						s_mins[i] = MIN(sbuf[sbuf_start], s_mins[i]);
//						s_maxs[i] = MAX(sbuf[sbuf_start], s_maxs[i]);
//						sbuf_start++;
//					}
//				}
//			}
		}
	}

	/* write to temp results buffer */
	/* if masked pixel, write mask flag only; else write mask flag and data */

	output_rec[n]->masked = masked;
	//	ot_nbytes += sizeof(double);
	if (! masked) {

//			printf("%f ", z_s);
			output_rec[n]->elevation = elevation;
			output_rec[n]->z_0 = z_0;
			output_rec[n]->z_s = z_s;
			output_rec[n]->rho = rho;
			output_rec[n]->T_s_0 = T_s_0;
			output_rec[n]->T_s_l = T_s_l;
			output_rec[n]->T_s = T_s;
			output_rec[n]->h2o_sat = h2o_sat;
			output_rec[n]->layer_count = layer_count;

			output_rec[n]->R_n_bar = R_n_bar;
			output_rec[n]->H_bar = H_bar;
			output_rec[n]->L_v_E_bar = L_v_E_bar;
			output_rec[n]->G_bar = G_bar;
			output_rec[n]->M_bar = M_bar;
			output_rec[n]->delta_Q_bar = delta_Q_bar;
			output_rec[n]->E_s_sum = E_s_sum;
			output_rec[n]->melt_sum = melt_sum;
			output_rec[n]->ro_pred_sum = ro_pred_sum;

		//		ot_nbytes += TBANDS * sizeof(double);
	}
}
