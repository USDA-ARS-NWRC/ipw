#include "ipw.h"
#include "pgm.h"

void
find_neighbors (
	double		northing,	/* northing of output pixel	 */
	double		easting,	/* easting of output pixel	 */
	int		npoints,	/* # input data points		 */
	struct point   *points,		/* linked list of input points	 */
	int		nclosest,	/* # neighbors in interp	 */
	struct neighbor *neighbors,	/* array of neighbors		 */
	float 		val)            /* value of current point        */
{
	int		neibcount = 0;  /* count of valid neighbors      */
	int		zeroflag = 0;   /* count of zero distances       */
	int		dropflag = 0;   /* for dropping crossval pts     */
	int		p;		/* points loop counter		 */
	int		n;		/* neighbors loop counter	 */
	int		max;		/* index of farthest neighbor	 */
	double		dy;		/* distance in y direction	 */
	double		dx;		/* distance in x direction	 */
	double		dist2;		/* distance squared		 */
	double		max_dist2;	/* maximum distance squared	 */
	struct point   *node;		/* point considered		 */


	max_dist2 = 0;
	node = points;

   /* initialize neighbors list with first <nclosest> points */

/***&&& orig:
	for (p = 0; p < nclosest; p++) {
***/

	for (p = 0; neibcount < nclosest && p < npoints; p++) {
		if (node->value == nodataval) {
			node = node->next;
			continue;
		}
		dy = node->northing - northing;
                dx = node->easting  - easting;
		dist2 = dy * dy + dx * dx;

/***
fprintf(stderr,"val = %.2f  node->value = %.2f  dist = %.2f \n", 
	val, node->value, sqrt(dist2));
***/

		if (dist2 == 0.0) {   
			zeroflag++;
		 	if (val == node->value) { /* then drop this point */
			    dropflag++;
			    node = node->next;
			    continue;
			}
		}

/*&&& below, substituted neibcount for p  rd. */
		  	
                neighbors[neibcount].dist2 = dist2;

/***&&&&& added next line (possible vi fuckup earlier) ****/
                neighbors[neibcount].value = node->value;

		if (dist2 > max_dist2) {
			max_dist2 = dist2;
			max = neibcount;
		}
		node = node->next;
		neibcount++;
	}

	if ( neibcount != nclosest) {
	    fprintf(stderr, "ERROR: Couldn't find enough valid data points.\n");
    	    fprintf(stderr, "       Too much missing data in point set.\n");
	    exit(1);
	}
		

   /* calculate distance to remaining points, keeping closest neighbors */

	for ( ; p < npoints; p++) {

		if (node->value == nodataval) {
			node = node->next;
			continue;
		}
		dy = node->northing - northing;
                dx = node->easting  - easting;
		dist2 = dy * dy + dx * dx;

		if (dist2 == 0.0) {   
			zeroflag++;
		 	if (val == node->value) { /* then drop this point */
			    dropflag++;
			    node = node->next;
			    continue;
			}
		}

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
		neibcount++;
	}

/* dump neib array 
fprintf(stderr, "Neighbor dump: \n");
for (p=0 ; p < nclosest; p++) {
    fprintf(stderr, "  neib[%d]: %.1f  %.2f\n", p, neighbors[p].dist2,
	neighbors[p].value);
}
*/


	/* if crossval, report on coincident points in x,y or x,y,z */

	if (jackflag == 1) {
    	    if (dropflag == 0) {
	  	fprintf(stderr, "this point can't exist: %f %f %f ",
  	  	    easting, northing, val);
    		error ("Bad coord data: dropflag = 0 in find_neighbors.\n");
    	    }
    	    if (zeroflag > 1) {
    		fprintf(stderr, "%f %f %f NOTE: coincident",
    			easting, northing, val);
    		if (dropflag > 1) { fprintf(stderr, " in x,y,z.\n"); 
    		} else {            fprintf(stderr, " in x,y.\n"); }
    	    }
        }
}
