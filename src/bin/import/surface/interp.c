/*
** NAME
** 	interp -- interpolate value from nearest neighbors
** 
** SYNOPSIS
**	double interp (d1, nclosest, neighbors)
**	int d1;
**	int nclosest;
**	struct neighbor *neighbors;
** 
** DESCRIPTION
**	interp interpolates for the value using either inverse-distance
**	squared or simple inverse distance weighting with the given
**	nclosest neighbors.
** 
** RETURN VALUE
**	interpolated value
** 
*/

#include "ipw.h"
#include "pgm.h"
#include <math.h>

double
interp (
	int		d1,		/* distance weighting flag	 */
	int		nclosest,	/* # neighbors in interp	 */
	struct neighbor *neighbors)	/* array of neighbors		 */
{
	int		n;		/* neighbors loop counter	 */
	double		dist;		/* distance to neighbor		 */
	double		sum1;		/* numerator sum		 */
	double 		sum2;		/* denominator sum		 */

	sum1 = 0.0;
	sum2 = 0.0;

	if (d1) {
		for (n = 0; n < nclosest; n++) {

			if (neighbors[n].dist2 > 0.0) {
				dist = sqrt(neighbors[n].dist2);
				sum1 += neighbors[n].value / dist;
				sum2 += 1.0/dist;
			} else {
				sum1 = neighbors[n].value;
				sum2 = 1.0;
				break;
			}
		}
	}
	else {
		for (n = 0; n < nclosest; n++) {

			if ((dist = neighbors[n].dist2) > 0.0) {
				sum1 += neighbors[n].value / dist;
				sum2 += 1.0/dist;
			} else {
				sum1 = neighbors[n].value;
				sum2 = 1.0;
				break;
			}
		}
	}

	return ((double) (sum1 / sum2));
}
