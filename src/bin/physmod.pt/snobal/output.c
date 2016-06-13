/*
 ** NAME
 **      output -- writes output records for 2-layer snowmelt model
 **
 ** SYNOPSIS
 **      #include "pgm.h"
 **
 **      void
 **	output(void)
 **
 ** DESCRIPTION
 **      Writes output record for 2-layer snowmelt model snobal.c.
 **
 ** GLOBAL VARIABLES READ
 **	cc_s
 **	cc_s_0
 **	cc_s_l
 **	current_time
 **	delta_Q_bar
 **	delta_Q_0_bar
 **	E_s_sum
 **	G_bar
 **	G_0_bar
 **	H_bar
 **	L_v_E_bar
 **	M_bar
 **	m_s
 **	m_s_0
 **	m_s_l
 **	melt_sum
 **	out
 **	out_file
 **	R_n_bar
 **	rec_count
 **	rho
 **	ro_pred_sum
 **	T_s
 **	T_s_0
 **	T_s_l
 **	temps_in_C
 **	time_since_out
 **	z_s
 **	z_s_0
 **	z_s_l
 **
 ** GLOBAL VARIABLES MODIFIED
 **	curr_time_hrs
 **
 */

#include "ipw.h"
#include "snobal.h"
#include "pgm.h"
#include "envphys.h"

void
output(void)

{
	/***    write output file       ***/

	curr_time_hrs = SEC_TO_HR(current_time);

	if (out_file) {
		//		/* time */
		//		fprintf(out, "%g", curr_time_hrs);
		//		/* energy budget terms */
		//		fprintf(out, " %.1f %.1f %.1f %.1f %.1f %.1f",
		//				R_n_bar, H_bar, L_v_E_bar, G_bar, M_bar, delta_Q_bar);
		//
		//		/* layer terms */
		//		fprintf(out, " %.1f %.1f",
		//				G_0_bar, delta_Q_0_bar);
		//
		//		/* heat storage and mass changes */
		//		fprintf(out, " %.6e %.6e %.6e",
		//				cc_s_0, cc_s_l, cc_s);
		//		fprintf(out, " %.5f %.5f %.5f",
		//				E_s_sum, melt_sum, ro_pred_sum);
		//
		//		/* runoff error if data included */
		//		if (ro_data)
		//			fprintf(out, " %.1f",
		//					(ro_pred_sum - (ro * time_since_out)));
		//
		//		/* sno properties */
		//		fprintf(out, " %.3f %.3f %.3f %.1f",
		//				z_s_0, z_s_l, z_s, rho);
		//		fprintf(out, " %.1f %.1f %.1f %.1f",
		//				m_s_0, m_s_l, m_s, h2o);
		//		if (temps_in_C)
		//			fprintf(out, " %.2f %.2f %.2f\n",
		//					K_TO_C(T_s_0), K_TO_C(T_s_l), K_TO_C(T_s));
		//		else
		//			fprintf(out, " %.2f %.2f %.2f\n",
		//					T_s_0, T_s_l, T_s);
		/* time */
		fprintf(out, "%g", curr_time_hrs);
		/* energy budget terms */
		fprintf(out, " %.3f %.3f %.3f %.3f %.3f %.3f",
				R_n_bar, H_bar, L_v_E_bar, G_bar, M_bar, delta_Q_bar);

		/* layer terms */
		fprintf(out, " %.3f %.3f",
				G_0_bar, delta_Q_0_bar);

		/* heat storage and mass changes */
		fprintf(out, " %.9e %.9e %.9e",
				cc_s_0, cc_s_l, cc_s);
		fprintf(out, " %.8f %.8f %.8f",
				E_s_sum, melt_sum, ro_pred_sum);

		/* runoff error if data included */
		if (ro_data)
			fprintf(out, " %.3f",
					(ro_pred_sum - (ro * time_since_out)));

		/* sno properties */
		fprintf(out, " %.6f %.6f %.6f %.3f",
				z_s_0, z_s_l, z_s, rho);
		fprintf(out, " %.3f %.3f %.3f %.3f",
				m_s_0, m_s_l, m_s, h2o);
		if (temps_in_C)
			fprintf(out, " %.5f %.5f %.5f\n",
					K_TO_C(T_s_0), K_TO_C(T_s_l), K_TO_C(T_s));
		else
			fprintf(out, " %.5f %.5f %.5f\n",
					T_s_0, T_s_l, T_s);
	}
	else {
		/* step_time and record number */
		fprintf(stdout, "\nrecord # %d:\ttime=%.2f;\n",
				rec_count, curr_time_hrs);
		/* energy budget terms */
		fprintf(stdout, "energy terms:\t");
		fprintf(stdout, "R_n=%.1f; H=%.1f; L_v_E=%.1f; G=%.1f; M=%.1f; delta_Q=%.1f;\n",
				R_n_bar, H_bar, L_v_E_bar, G_bar, M_bar, delta_Q_bar);
		fprintf(stdout, "layer terms:\t");
		fprintf(stdout, "G_0=%.1f; delta_Q_0=%.1f;\n",
				G_0_bar, delta_Q_0_bar);

		/* mass changes and heat storage */
		fprintf(stdout, "cc terms:\t");
		fprintf(stdout, "cc_s_0=%.6e; cc_s_l=%.6e; cc_s=%.6e;\n",
				cc_s_0, cc_s_l, cc_s);
		fprintf(stdout, "mass terms:\t");
		fprintf(stdout, "E_s=%.5f; melt=%.5f; ro_predict=%.5f;\n",
				E_s_sum, melt_sum, ro_pred_sum);

		/* runoff error if data included */
		if (ro_data)
			fprintf(stdout, "runoff error:\t%.1f;\n",
					ro_pred_sum - (ro * time_since_out));

		/* sno properties */
		fprintf(stdout, "snow depths:\t");
		fprintf(stdout, "z_s_0=%.4f; z_s_l=%.4f; z_s=%.4f;\n",
				z_s_0, z_s_l, z_s);
		fprintf(stdout, "density, mass:\t");
		fprintf(stdout, "rho=%.1f; m_s_0=%.1f; m_s_l=%.1f; m_s=%.1f;\n",
				rho, m_s_0, m_s_l, m_s);
		fprintf(stdout, "h2o, temps:\t");
		fprintf(stdout, "h2o=%.1f; ", h2o);
		if (temps_in_C) 
			fprintf(stdout, "T_s_0=%.2f; T_s_l=%.2f; T_s=%.2f;\n",
					K_TO_C(T_s_0), K_TO_C(T_s_l), K_TO_C(T_s));
		else
			fprintf(stdout, "T_s_0=%.2f; T_s_l=%.2f; T_s=%.2f;\n",
					T_s_0, T_s_l, T_s);
	}

	time_since_out = 0.0;
}
