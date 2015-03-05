/*
** NAME
**      sn_err -- prints differences in snow parameters at update time
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	sn_err(
**	    double  new_z_s,	  |* new snowcover depth *|
**	    double  new_rho,	  |* new snowcover density *|
**	    double  new_T_s_0,	  |* new surface layer temperature *|
**	    double  new_T_s,	  |* new snowcover temperature *|
**	    double  new_h2o_sat)  |* new snowcover % liquird h2o saturation *|
**
** DESCRIPTION
**      This routine computes the difference (error) between the current
**	snow parameters and the new values that've been just read in
**	for an update.
**
** GLOBAL VARIABLES READ
**	h2o
**	rho
**	T_s_0
**	T_s
**	time_s
**	z_s
**
** GLOBAL VARIABLES MODIFIED
*/

#include "ipw.h"
#include "pgm.h"
#include "snow.h"

void
sn_err(
	double  new_z_s,	/* new snowcover depth */
	double  new_rho,	/* new snowcover density */
	double  new_T_s_0,	/* new surface layer temperature */
	double  new_T_s,	/* new snowcover temperature */
	double  new_h2o_sat)	/* new snowcover % liquird h2o saturation */
{
        double  new_h2o_vol;	/* new liquid h2o content as volume ratio */
        double  new_h2o;	/* new liquid h2o content as specific mass */

	new_h2o_vol = new_h2o_sat * max_h2o_vol;
	new_h2o = H2O_LEFT(new_z_s, new_rho, new_h2o_vol);

        printf("time_diff=%g; ",  SEC_TO_HR(time_s));
	printf("diff_z_s=%g; ",   z_s - new_z_s);
	printf("diff_rho=%g; ",   rho - new_rho);  
	printf("diff_m_s=%g; ",   m_s - (new_z_s * new_rho));

	/*
	 *  Want to output difference between predicted and measured
	 *  temperatures in degrees C or K depending upon how the flag
	 *  "temps_in_C" is set.  These internal values are in degrees K;
	 *  however, we don't need to convert the temperatures since
	 *  the difference between two temps is the same magnitude
	 *  for degrees C or degrees K.
	 */
	printf("diff_T_s_0=%g; ", T_s_0 - new_T_s_0);
	printf("diff_T_s=%g; ",   T_s - new_T_s);

	printf("diff_h2o=%g;\n",  h2o - new_h2o);
}
