/*
** NAME
** 	find_neighbors -- returns nearest neighbors to given location
** 
** SYNOPSIS
**	void
**	find_neighbors (northing, easting, npoints, points, nclosest, neighbors)
**	double northing, easting;
**	int npoints;
**	struct point *points;
**	int nclosest;
**	struct neighbor *neighbors;
** 
** DESCRIPTION
**	find_neighbors searches the list of points for the <nclosest> points
**	to the given northing and easting.  The values and distances are
**	returned in the neighbors array.
** 
*/

#include "ipw.h"
#include "pgm.h"

void
find_neighbors (
	double		northing,	/* northing of output pixel	 */
	double		easting,	/* easting of output pixel	 */
	int		npoints,	/* # input data points		 */
	struct point   *points,		/* linked list of input points	 */
	int		nclosest,	/* # neighbors in interp	 */
	struct neighbor *neighbors)	/* array of neighbors		 */
{
	int		p;		/* points loop counter		 */
	int		n;		/* neighbors loop counter	 */
	int		max =0;		/* index of farthest neighbor	 */
	double		dy;		/* distance in y direction	 */
	double		dx;		/* distance in x direction	 */
	double		dist2;		/* distance squared		 */
	double		max_dist2;	/* maximum distance squared	 */
	struct point   *node;		/* point considered		 */


	max_dist2 = 0;
	node = points;

   /* initialize neighbors list with first <nclosest> points */

	for (p = 0; p < nclosest; p++) {
		dy = node->northing - northing;
                dx = node->easting  - easting;
		dist2 = dy * dy + dx * dx;

                neighbors[p].dist2 = dist2;
		neighbors[p].value = node->value;

		if (dist2 > max_dist2) {
			max_dist2 = dist2;
			max = p;
		}
		node = node->next;
	}

   /* calculate distance to remaining points, keeping closest neighbors */

	for ( ; p < npoints; p++) {

		dy = node->northing - northing;
                dx = node->easting  - easting;
		dist2 = dy * dy + dx * dx;

		/* if distance to this point < max, replace max */

		if (dist2 < max_dist2) {
			neighbors[max].dist2 = dist2;
			neighbors[max].value = node->value;
		}

		/* find new max */

		max = 0;
		max_dist2 = neighbors[0].dist2;
		for (n = 1; n < nclosest; n++) {
			if (neighbors[n].dist2 > max_dist2) {
				max = n;
				max_dist2 = neighbors[n].dist2;
			}
		}

		node = node->next;
	}
}
