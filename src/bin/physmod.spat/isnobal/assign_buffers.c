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
	int		masked,		/* is point masked?		   */
	int	        output)		/* writing output image this step? */
{
	fpixel_t       *bufp;		/* -> in line buffers		 */
	int		i;		/* band loop counter		 */


   /* if output this time step, set output buffer bands */

	if (output) {
		bufp = embuf_p;

		if (masked) {
			for (i = 0; i < EMBANDS; i++) {
				*embuf_p++ = NO_DATA;
			}
		} else {
			*embuf_p++ = R_n_bar;
			*embuf_p++ = H_bar;
			*embuf_p++ = L_v_E_bar;
			*embuf_p++ = G_bar;
			*embuf_p++ = M_bar;
			*embuf_p++ = delta_Q_bar;
			*embuf_p++ = E_s_sum;
			*embuf_p++ = melt_sum;
			*embuf_p++ = ro_pred_sum;
			*embuf_p++ = cc_s;

			/* keep min & max of each band */
	
			if (first_em_pix) {
				first_em_pix = FALSE;
				for (i=0; i < EMBANDS; i++) {
					em_mins[i] = *bufp;
					em_maxs[i] = *bufp;
					bufp++;
				}
			} else {
				for (i=0; i < EMBANDS; i++) {
					em_mins[i] = MIN(*bufp, em_mins[i]);
					em_maxs[i] = MAX(*bufp, em_maxs[i]);
					bufp++;
				}
			}
		}

		/* do the same for snow output image */

		if (masked) {
			for (i = 0; i < SBANDS; i++) {
				*sbuf_p++ = NO_DATA;
			}
		} else {
			bufp = sbuf_p;

			*sbuf_p++ = z_s;
			*sbuf_p++ = rho;
			*sbuf_p++ = m_s;
			*sbuf_p++ = h2o;
			*sbuf_p++ = T_s_0 - FREEZE;
			*sbuf_p++ = T_s_l - FREEZE;
			*sbuf_p++ = T_s   - FREEZE;
			*sbuf_p++ = z_s_l;
			*sbuf_p++ = h2o_sat;
	
			/* keep min & max of each band */
	
			if (first_snow_pix) {
				first_snow_pix = FALSE;
				for (i=0; i < SBANDS; i++) {
					s_mins[i] = *bufp;
					s_maxs[i] = *bufp;
					bufp++;
				}
			} else {
				for (i=0; i < SBANDS; i++) {
					s_mins[i] = MIN(*bufp, s_mins[i]);
					s_maxs[i] = MAX(*bufp, s_maxs[i]);
					bufp++;
				}
			}
		}
	}

   /* write to temp results buffer */
   /* if masked pixel, write mask flag only; else write mask flag and data */

	*otbuf_p++ = masked;
	ot_nbytes += sizeof(double);
	if (! masked) {
		*otbuf_p++ = elevation;
		*otbuf_p++ = z_0;
		*otbuf_p++ = z_s;
		*otbuf_p++ = rho;
		*otbuf_p++ = T_s_0;
		*otbuf_p++ = T_s_l;
		*otbuf_p++ = T_s;
		*otbuf_p++ = h2o_sat;

		*otbuf_p++ = R_n_bar;
		*otbuf_p++ = H_bar;
		*otbuf_p++ = L_v_E_bar;
		*otbuf_p++ = G_bar;
		*otbuf_p++ = M_bar;
		*otbuf_p++ = delta_Q_bar;
		*otbuf_p++ = E_s_sum;
		*otbuf_p++ = melt_sum;
		*otbuf_p++ = ro_pred_sum;

		ot_nbytes += TBANDS * sizeof(double);
	}
}
