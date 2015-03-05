/*
** NAME
** 	interp -- interpolate value from nearest neighbors
** 
** SYNOPSIS
**	double interp (nclosest, neighbors)
**	int nclosest;
**	struct neighbor *neighbors;
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
	int		nclosest)	/* # neighbors in interp	 */
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

	return ((double) (sum1 / sum2));
}
 
