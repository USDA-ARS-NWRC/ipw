/*
** NAME
** 	precip_event -- read precip file and check for precip event
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	bool_t
**	precip_event(
**	    float	curr_time,	|* current time (hrs)		  *|
**	    char       *pre_img)	|* -> buffer to hold precip image *|
** 
** DESCRIPTION
** 	precip_event reads the precipitation data file and checks for a
**	precip event within the current data timestep.  If there is a
**	precip event, the name of the corresponding precipitation
**	image file is returned.
** 
** RESTRICTIONS
** 
** RETURN VALUE
**	TRUE	There is a precip event for the current data timestep.
**		The name of the precip image has been stored in the
**		output parameter 'pre_img'.
**
**	FALSE	No precip event for the current data timestep.
** 
** GLOBAL VARIABLES READ
** 	pfp
**
** GLOBAL VARIABLES MODIFIED
*/

#include "ipw.h"
#include "pgm.h"
#include "snobal.h"

static int   first_time = TRUE;
static int   more_precip = TRUE;
static float data_tstep;		/* data timestep in hours	   */
static float next_time;			/* time of next precip event (hrs) */

bool_t
precip_event(
	float		curr_time,	/* current time (hrs)		  */
	char	       *pre_img)	/* -> buffer to hold precip image */
{
	int		nread;		/* # values read by fscanf()	 */


   /* if no more precip events, return FALSE */

	if (!more_precip)
		return (FALSE);

   /* if first time called, read time of first precip event */

	if (first_time) {
		first_time = FALSE;
		data_tstep = SEC_TO_HR(tstep_info[DATA_TSTEP].time_step);
		if (fscanf(pfp, "%f", &next_time) != 1) {
			error("bad or missing data in precip file");
		}
	}

   /* skip events that have already passed - warn user */

	while (curr_time > next_time) {

		warn ("precip event at %f hours ignored: time already passed",
			next_time);

		/* skip name of image for this event */

		if (fscanf(pfp, "%s", pre_img) != 1) {
			error("bad or missing data in precip file");
		}

		if ((nread = fscanf(pfp, "%f", &next_time)) == EOF) {
			more_precip = FALSE;
			return (FALSE);
		} else {
			if (nread != 1) {
				error("bad or missing data in precip file");
			}
		}
	}

   /* check if the next precip event is in this time step */

	if (next_time < (curr_time + data_tstep)) {
	
		/* read name of image for this event */

		if (fscanf(pfp, "%s", pre_img) != 1) {
			error("bad or missing data in precip file");
		}

		/* read ahead for next event time */

		if ((nread = fscanf(pfp, "%f", &next_time)) == EOF) {
			more_precip = FALSE;
		} else {
			if (nread != 1) {
				error("bad or missing data in precip file");
			}
		}

		return (TRUE);
	}

	return (FALSE);
}
