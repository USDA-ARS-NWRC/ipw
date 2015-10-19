/*
** NAME
** 	snow_image -- create snow-conditions output image
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	snow_image(
**		int	 step,		|* current step #		   *|
**		char	*tempfile)	|* temporary file with pixel data  *|
** 
** DESCRIPTION
** 	snow_image creates a new snow-conditions output image.  The name of
**	the new image is "snow_prefix.N" where N is the given step number.
**	The pixel data for the new image is copied from the given temporary
**	file.
** 
** RETURN VALUE
** 
** GLOBALS ACCESSED
**	compress_cmd
**	s_maxs
**	s_mins
**	sbuf
**	snow_prefix
*/

#include <stdlib.h>

#include "ipw.h"
#include "pgm.h"

/*
 * annotations for bands of snow output images
 */

char *s_annot[] = {
	"z_s: predicted snow depth (m)",
	"rho: predicted avg. snow density (kg/m^3)",
	"m_s: predicted specific mass (kg/m^2)",
	"h2o: predicted avg. liquid water content (kg)",
	"T_s_0: predicted surface temperature (C)",
	"T_s_l: predicted lower layer temperature (C)",
	"T_s: predicted avg. snow temperature (C)",
	"z_s_l: lower layer depth (m)",
	"h2o_sat: % liquid h2o saturation",
};

/*
 *  Units for each band in snow output image
 */
char *s_units[] = {
	"m",
	"kg/m^3",
	"kg/m^2",
	"kg",
	"C",
	"C",
	"C",
	"m",
	"%"
};


void
snow_image(
	int	 step,		/* current step #			*/
	char	*tempfile)	/* temporary file with pixel data	*/
{
	char	 filename[255];	/* name for new snow image		*/
	int	 fd;		/* file descriptor for snow image	*/
	char	 syscmd[512];	/* name for new snow image		*/

 
	sprintf(filename, "%s.%0*d", snow_prefix, nDigits, step);
	fd = output_image(filename, SBANDS, s_units, s_annot, s_mins, s_maxs);

	copy_image(tempfile, SBANDS, sbuf, fd);

	if (compress_cmd != NULL) {
		sprintf(syscmd, "%s -9 %s &", compress_cmd, filename);
		system(syscmd);
	}
}
