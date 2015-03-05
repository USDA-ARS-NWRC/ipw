/*
** NAME
** 	get_points -- read in points from points input file
**		      allocate point and neighbors arrays
** 		      read matrix file into memory
** 
*/

#include "ipw.h"
#include "pgm.h"

int 
get_points(
	FILE		*ptfile,
	int		*npoints,
	float		*min, 
      	float		*max)
{
    int i, j, items, na = 0;

    /* get n (number of points) from point file */
    items = Get_items(ptfile);
    rewind(ptfile);
    if( (items % 3) != 0) error("Wrong number of items in point_file.");
    *npoints = items / 3;

    /* allocate memory */
 
    /* array of npoints POINT structures */
    points = (POINT *) ecalloc(*npoints, sizeof(POINT));
    if(points == NULL) error("Memory allocation error on points array.");
 
    /* array of structures holding neighbor id's and distances */
    neighbors = (NEIB *) ecalloc(*npoints, sizeof(NEIB));
    if(neighbors == NULL) error("Memory allocation error on neighbors array.");
 
    /* read points from point_file */
    for(i=0, j=3; i<*npoints; i++) {
        j = fscanf(ptfile, "%lf %lf %lf",
            &points[i].x, &points[i].y,  &points[i].z);
        if(j != 3) error("Bad read from point_file.");
        
        if(points[i].z == naflag) na++;
    }

    /* get min/max values */
    for(i=0; i<*npoints; i++) {
	if(points[i].z == naflag) continue;
        *min = *max = points[i].z;
	break;
    }
    for(j=i; j<*npoints; j++) {
	if(points[j].z == naflag) continue;
	*min = MIN (points[j].z, *min);
	*max = MAX (points[j].z, *max);
    }

    return(na);
 
} /* end function */
/*****************************************************************************/
int
Get_items(
	FILE	*fp)
{
    int items = 0, result = 1;
    float temp;
 
    while(result != EOF) {
        result = fscanf(fp, "%f", &temp);
        if(result == 1) items++;
        else if(result == 0) error("Bad read from file. (non-numeric value?)");
    }
    return(items);
}

