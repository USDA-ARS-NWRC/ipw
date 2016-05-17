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
		int	 step,						/* current step #			*/
		OUTPUT_REC *output_rec[],		/* ouput sturcture */
		int nbits)							/* number of bits */
{
	char filename[255];		/* name for new snow image		*/
	int	 fd;				/* file descriptor for snow image	*/
	char syscmd[512];		/* name for new snow image		*/
	int i, j, N, start;
	bool_t first_pix;
	int s_samps;


	/* find the min and maximums */
	N = nlines * nsamps;
	start = 0;
	first_pix = TRUE;
	s_samps = N * SBANDS;

	for (j = 0; j < N; j++) {

		// loop through each pixel that isn't masked
		if (!output_rec[j]->masked) {

			start = j*SBANDS;

			// if it's the first pixel
			if (first_pix) {
				for (i=0; i < SBANDS; i++) {
					s_mins[i] = sbuf[start];
					s_maxs[i] = sbuf[start];
					start++;
				}
				first_pix = FALSE;
			} else {
				for (i=0; i < SBANDS; i++) {
					s_mins[i] = MIN(sbuf[start], s_mins[i]);
					s_maxs[i] = MAX(sbuf[start], s_maxs[i]);
					start++;
				}
			}
		}
	}

	sprintf(filename, "%s.%0*d", snow_prefix, nDigits, step);
	fd = output_image(filename, SBANDS, s_units, s_annot, s_mins, s_maxs, nbits);

	//	copy_image(tempfile, SBANDS, sbuf, fd);
	if (fpvwrite (fd, sbuf, N) != N) {
		error ("write error output snow image");
	}

	(void) fpclose(fd);

	if (compress_cmd != NULL) {
		sprintf(syscmd, "%s -9 %s &", compress_cmd, filename);
		system(syscmd);
	}
}
