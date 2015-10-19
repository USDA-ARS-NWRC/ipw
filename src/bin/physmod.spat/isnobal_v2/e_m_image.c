/*
** NAME
** 	e_m_image -- create energy/mass output image
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	e_m_image(
**		int	 step,		|* current step #		   *|
**		char	*tempfile)	|* temporary file with pixel data  *|
** 
** DESCRIPTION
** 	e_m_image creates a new energy/mass output image.  The name of
**	the new image is "em_prefix.N" where N is the given step number.
**	The pixel data for the new image is copied from the given temporary
**	file.
** 
** RETURN VALUE
** 
** GLOBALS ACCESSED
**	compress_cmd
**	em_maxs
**	em_mins
**	em_prefix
**	embuf
*/

#include <stdlib.h>

#include "ipw.h"
#include "pgm.h"

/*
 * annotations for bands of energy/mass output images
 */

char *em_annot[] = {
	"R_n: net allwave rad (W/m^2)",
	"H: sensible heat transfer (W/m^2)",
	"L_v_E: latent heat exchange (W/m^2)",
	"G: snow/soil heat exchange (W/m^2)",
	"M: advected heat from precip. (W/m^2)",
	"delta_Q: sum of energy balance terms (W/m^2)",
	"E_s: evaporation (kg/m^2)",
	"melt: melt (kg/m^2)",
	"ro_predict: predicted runoff (kg, or mm/m^2)",
	"cc_s: cold content (J/m^2)"
};

/*
 *  Units for each band in energy/mass output image
 */

char *em_units[] = {
	"W/m^2",
	"W/m^2",
	"W/m^2",
	"W/m^2",
	"W/m^2",
	"W/m^2",
	"kg/m^2",
	"kg/m^2",
	"kg",
	"J/m^2" 
};


void
e_m_image(
	int	 step,		/* current step #			*/
	char	*tempfile)	/* temporary file with pixel data	*/
{
	char	 filename[255];	/* name for new energy/mass image	*/
	int	 fd;		/* file descriptor for energy/mass img	*/
	char	 syscmd[512];	/* name for new energy/mass image	*/

 
	sprintf(filename, "%s.%0*d", em_prefix, nDigits, step);
	fd = output_image(filename, EMBANDS, em_units, em_annot, em_mins,
			  em_maxs);

	copy_image(tempfile, EMBANDS, embuf, fd);

	if (compress_cmd != NULL) {
		sprintf(syscmd, "%s -9 %s &", compress_cmd, filename);
		system(syscmd);
	}
}
