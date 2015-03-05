/*
** NAME
** 	get_points -- file containing X,Y pairs for iinterp
** 
** SYNOPSIS
**	get_points (fp, npoints, xval, yval)
**	FILE *fp;
**	int npoints;
**	fpixel_t *xval, *yval;
** 
** DESCRIPTION
** 	get_points reads the given input file and returns the X, Y pairs.
*/

#include "ipw.h"
#include "pgm.h"

void
get_points (
	FILE		*fp,		/* file pointer to open points file */
	int		 npoints,	/* number of points                 */
	fpixel_t	*xval,		/* return X-values		    */
	fpixel_t	*yval)		/* return Y-values		    */
{
	int		nread;		/* number of data values read       */
	int		n;		/* current # points read	    */
	int		i;		/* loop counter			    */
	float		dummy;		/* dummy value			    */

	n = 0;

	while (n < npoints) {

		if ((nread = fscanf (fp, "%f %f", &xval[n], &yval[n])) != 2) {
			error ("missing or illegal data in points file: %d points successfully read", n);
		}
		n++;
	}

	/* make sure no more points in file */

	if (fscanf (fp, "%f", &dummy) != EOF) {
		warn ("more than %d points in point file - remainder ignored",
			npoints);
	}

	/* check for sorting and continuity */

	for (i = 1; i < npoints; i++) {
		if (xval[i] <= xval[i-1]) {
			error ("points not sorted in ascending X-order");
		}
	}

}
