/**************************************************************************
Find nearest neighbors.
**************************************************************************/
#include "ipw.h"
#include "pgm.h"

void Find_neibs(
	int	nneibs,
	int	npoints,
	double	cellx,
	double	celly,
	int	line,
	int	samp)
{

    int i, j, max_id;
    double  geodist();			  /* spherical distance function */
    double dx, dy, max_dist2 = 0;

    /* compute distances */
    for(i=0; i<npoints; i++) {
	if (parm.geoflag) {
	    neighbors[i].dist2 = 
		geodist(points[i].coslat, points[i].sinlat, 
		 	linetrig[line].cos, linetrig[line].sin,
			points[i].lonrad, samplonrad[samp]);
	} else {
	    dx = points[i].x - cellx;
	    dy = points[i].y - celly;
  	    neighbors[i].dist2 = dx * dx + dy * dy;
	}
  	neighbors[i].id = i;
    }

    /* initialize first nneibs closest neibs */
    for(i=0; i<nneibs; i++) {
	closest[i].id = neighbors[i].id;
	closest[i].dist2 = neighbors[i].dist2;
	if(closest[i].dist2 > max_dist2) {
	    max_dist2 = closest[i].dist2;
	    max_id    = i;
	}
    }
    
    /* find closest neibs */ 
    for(i=nneibs; i<npoints; i++) {
	if(neighbors[i].dist2 < max_dist2) {
	    closest[max_id].dist2 = neighbors[i].dist2;
	    closest[max_id].id    = neighbors[i].id;


	    /* find max of new set of closest neibs */
	    max_dist2 = closest[0].dist2;
	    max_id    = 0;
    	    for(j=1; j<nneibs; j++) {
	        if(closest[j].dist2 > max_dist2) {
		    max_dist2 = closest[j].dist2;
		    max_id    = j;
		}
	    }
	}
    }
} /* end function */
