/*
 ** NAME
 **      initialize - sets initial conditions for snobal model
 **
 ** SYNOPSIS
 **      #include "pgm.h"
 **
 **      void
 **	initialize(void)
 **
 ** DESCRIPTION
 **      This routine sets up some parameters that remain constant for the
 **	duration of the model run.  It also fetches the first records for
 **	the snow properties, measurement heights & depths, and precipitation
 **	to establish initial conditions.
 **
 ** GLOBAL VARIABLES READ
 **	elevation
 **	pr_file
 **
 ** GLOBAL VARIABLES MODIFIED
 **	mh_recs_done
 **	P_a
 **	pr_recs_done
 **	precip_data
 **	relative_hts
 **	ro_data
 **	time_since_out
 **	sn_recs_done
 */

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"

void
initialize(void)
{
	/**  set constant model parameters  **/

	/* air pressure */
	P_a = HYSTAT (SEA_LEVEL, STD_AIRTMP, STD_LAPSE, (elevation / 1000.0),
			GRAVITY, MOL_AIR);


	/**     get first records for initial conditions **/

	/*      initialize snow properties */
	get_sn_rec();

	/*      get measurement-height record */
	get_mh_rec();

	relative_hts = FALSE;

	/*      runoff data?    */
	/*
	 *  Instead of asking user,
	 *	
	 *	ro_data = getok("Do you have runoff data for this run?");
	 *
	 *  the previous version snobal just turned off this flag.
	 *  J. Domingo, 9 Feb 1996 
	 */
	ro_data = FALSE;


	/*      get precipitation record    */

	if (! pr_file) {
		precip_data = getok("Do you have precip data for this run?");
	}

	if (precip_data) {
		get_pr_rec();
	}

	out_func = output;
	time_since_out = 0.0;
}
