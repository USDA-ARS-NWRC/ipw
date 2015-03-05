/*
** NAME
** 	interp -- interpolate value from nearest neighbors
** 
** SYNOPSIS
**	double interp (nclosest, neighbors, dem_z)
**	int nclosest;
**	struct neighbor *neighbors;
**	fpixel_t dem_z;
** 
** DESCRIPTION
**	interp interpolates for the value using inverse distance squared
**	weighting with the given nclosest neighbors.
** 
** RETURN VALUE
**	interpolated value
** 
*/

#include "ipw.h"
#include "pgm.h"

double
interp (
	int		nclosest,	/* # neighbors in interp	 */
	struct neighbor *neighbors,	/* array of neighbors		 */
	fpixel_t	dem_z)		/* elevation of DEM cell	 */
{
	int		n;		/* neighbors loop counter	 */
	double		dist2;		/* distance to neighbor		 */
	double		sum1;		/* numerator sum		 */
	double 		sum2;		/* denominator sum		 */

	sum1 = 0.0;
	sum2 = 0.0;

	for (n = 0; n < nclosest; n++) {

		if ((dist2 = neighbors[n].dist2) > 0.0) {
			sum1 += neighbors[n].value / dist2;
			sum2 += 1.0/dist2;
		} else {
			sum1 = neighbors[n].value;
			sum2 = 1.0;
			break;
		}
	}

	return ((double) ( dem_z - (sum1 / sum2) ));

/*** below, to return distance to closest station (use -n 1)
	but the sqrt shit doesn't work. ???
dist2 = sqrt(neighbors[0].dist2) / 1000.0;
fprintf(stderr,"dist = %.2lf \n", dist2);
return( (float) dist2);
***/

}
