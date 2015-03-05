/****************************************************************************
READ_POINTS - reads point_file into array of POINT structures
****************************************************************************/
#include "ipw.h"
#include "pgm.h"
#include <math.h>
#include <string.h>

int Read_points(
	FILE     *ptfile,
	int       nneibs,
	STRVEC_T *annotp)
{
    int   n,	            /* Return value: number of points in ptfile */
          i, j, 
	  get_lines();
    char  fbuf[BUFSIZE],    /* file buffer (one line of ptfile) 	*/
	 *fbufp,   	    /* pointer to fbuf				*/
	  abuf1[BUFSIZE],   /* part 1 of annotation buffer 		*/
	  abuf2[BUFSIZE];   /* part 2 of annotation buffer         	*/
 
    /* get npoints from point file */

    n = get_lines(ptfile);
    if(n < nneibs) 
	error("points file has less than <neighbors> points.");
    rewind(ptfile);
 
    /* array of npoints POINT structures */ 

    points = (POINT *)malloc(n * sizeof(POINT));
    if(points == NULL) error("Memory allocation error on points array.");

    /* array of npoints NEIB structures */ 

    neighbors = (NEIB *)malloc(n * sizeof(NEIB));
    if(neighbors == NULL) error("Memory allocation error on neighbors array.");

    /* array of nneibs NEIB structures */ 

    closest = (NEIB *)malloc(nneibs * sizeof(NEIB));
    if(closest == NULL) error("Memory allocation error on closest array.");

    /* read points from point_file */

    for(i=0, j=2; i<n; i++) {

	/* read a line of the input file */
	fbufp = fgets(fbuf, BUFSIZE, ptfile);

	/* read coords as two doubles */
        j = sscanf(fbufp, "%lf %lf", &points[i].x, &points[i].y);
        if(j != 2) error("Bad read from point_file.");

	/* read coords as two strings (for header annotation) */
        j = sscanf(fbufp, "%s%s", abuf1, abuf2);
        if(j != 2) error("Bad read from point_file.");

	/* if using geo-distance, compute sin/cos and lon in radians */
	if(parm.geoflag) {
	    points[i].sinlat = sin(points[i].y/DEG);
    	    points[i].coslat = cos(points[i].y/DEG);
	    points[i].lonrad = points[i].x/DEG;
	}

	/* assemble annot string (abuf1 + space + abuf2) */

	strcat(abuf1, " ");
	strcat(abuf1, abuf2);

	/* add annot string to annotp */

	annotp = addsv(annotp, abuf1);

    }

    return(n);

} /* end function */
 
/******************************************************************************
GET_LINES - returns number of lines in a file.
******************************************************************************/
int get_lines(
	FILE	*fp)
{
    int lines = 0;
    char buf[BUFSIZE];
    char *foo;
 
    while( (foo = fgets(buf, BUFSIZE, fp)) != NULL) lines++;
 
    return(lines);
}
 

