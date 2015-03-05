/*
** NAME
** 	get_distance - compute nearest-neighbor distances
** 
** SYNOPSIS
**	void
**	get_distance (&&&)
**
*/

#include "ipw.h"
#include "pgm.h"

void
get_distance (
	double		cellx,		/* cell easting			*/
	double		celly,		/* cell northing		*/
	int		npoints,	/* # input data points		*/
	int		nclosest)	/* # neighbors in interp	*/
{
	int		p;		/* points loop counter		 */
	int		search_flag=0;  /* if 1, need to search 	 */
	double		dy;		/* distance in y direction	 */
	double		dx;		/* distance in x direction	 */
	double		dist2;		/* distance squared		 */
	double		gdist, geodist(); /* for spherical distances     */


 	/* if any stored neib is NA, search for neibs with the */
	/* find_neibs function and return */
 
	for (p = 0; p < nclosest; p++) {

	    if( (close_id[p]+1) > npoints ) 
		error("different # of points in points file vs. input image");

	    if(close_id[p] == -1) {
		search_flag = 1;
		find_neibs(nclosest, npoints, cellx, celly);
	        break;
	    }
	} 
	
	/* compute distances to closest neighbors */

	if(search_flag == 0) {

	    for (p = 0; p < nclosest; p++) {

	        if (parm.geoflag) {
            	    gdist = geodist(points[close_id[p]].y, 
				    points[close_id[p]].x, celly, cellx);
            	    dist2 = gdist * gdist;
        	} else {
            	    dx = points[close_id[p]].x - cellx;
	            dy = points[close_id[p]].y - celly;
                    dist2 = dy * dy + dx * dx;
	  	}

		neighbors[p].dist2 = dist2;
		neighbors[p].value = points[close_id[p]].z;
	    }

	}
}
/**************************************************************************
Find nearest neighbors.
**************************************************************************/
void
find_neibs(
	int		nclosest,
	int		npoints,
	double		cellx,
	double		celly)
{
 
    int p, n, q, max;
    double dx, dy, gdist, dist2, max_dist2 = 0;
    double  geodist();                    /* spherical distance function */

 
 /* initialize neighbors list with first <nclosest> points */
 /* q references all points; p references neighbor points  */
 
        for (p = 0, q = 0; p < nclosest; q++) {

		if (q >= npoints) {
		    error ("too much missing data for point %lf, %lf",
			    cellx, celly);
		}
		if (points[q].z == naflag) continue;

	        if (parm.geoflag) {
            	    gdist = geodist(points[q].y, points[q].x, celly, cellx);
            	    dist2 = gdist * gdist;
        	} else {
            	    dx = points[q].x - cellx;
	            dy = points[q].y - celly;
                    dist2 = dy * dy + dx * dx;
            	}

                neighbors[p].dist2 = dist2;
                neighbors[p].value = points[q].z;
 
                if (dist2 > max_dist2) {
                        max_dist2 = dist2;
                        max = p;
                }
		p++;
        }


 /* calculate distance to remaining points, keeping closest neighbors */
 
        for ( ; q < npoints; q++) {

		if (points[q].z == naflag) continue;
	        if (parm.geoflag) {
            	    gdist = geodist(points[q].y, points[q].x, celly, cellx);
            	    dist2 = gdist * gdist;
        	} else {
            	    dx = points[q].x - cellx;
	            dy = points[q].y - celly;
                    dist2 = dy * dy + dx * dx;
            	}
 
                /* if distance to this point < max, replace max */
 
                if (dist2 < max_dist2) {
                        neighbors[max].dist2 = dist2;
                        neighbors[max].value = points[q].z;
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
		p++;
        }

} /* end function */
