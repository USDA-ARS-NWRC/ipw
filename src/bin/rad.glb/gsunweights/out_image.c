/*
**
** NAME
** 	out_image -- create output image file; copy temporary output file
** 
** SYNOPSIS
** 
**	void out_image (tmpname, ikt, prefix, nlines, nsamps, nbits, mins, maxs)
**	char *tmpname;
**	int ikt;
**	char *prefix;
**	int nlines, nsamps, *nbits;
**	double *mins, *maxs;
** 
** DESCRIPTION
** 	out_image creates an output image with the given filename prefix
**	and index extension and copies the temporary output file to it.
** 
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
out_image (
	char	       *tmpname,	/* name of temporary file	 */
	int		ikt,		/* kronrod point index		 */
	char	       *prefix,		/* prefix for output image name	 */
	int		nlines,		/* # lines in image		 */
	int		nsamps,		/* # samples in image		 */
	int	       *nbits,		/* # bits/band in image		 */
	double	       *mins,		/* min for each band		 */
	double	       *maxs)		/* max for each band		 */
{
	char		filename[255];	/* name of output image		 */
	int		fdo;		/* output image file desc	 */


   /* open output image file */

	sprintf (filename, "%s.%02d", prefix, ikt);
	fdo = uwopen (filename);
	if (fdo == ERROR) {
		error ("Can't open output image file %s", filename);
	}

   /* write headers */

	write_headers (fdo, nbits, mins, maxs);

   /* copy temp file to output image */

	output (tmpname, OBANDS, fdo);

   /* close files */

	fpclose (fdo);
}
