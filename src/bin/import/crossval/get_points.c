/*
** NAME
** 	get_points -- read in points from points input file
** 
** SYNOPSIS
**	void
**	get_points (ifp, points, npoints, min, max)
**	FILE *ifp;
**	struct point **points;
**	int *npoints;
**	float *min, *max;
** 
** DESCRIPTION
**	get_points reads in the easting, northing, data value triples from
**	the given input file and returns them in a linked list of point
**	structures.  The number of points and the min and max data value are 
**	also returned.
**
**	If the crossval option is set, get_points creates an additional
**	array of PTARRAY structures, which hold both string and float
**	values for easting, northing, and value.
** 
*/

#include "ipw.h"
#include "pgm.h"

void
get_points (
	FILE	       *ifp,		/* file ptr to input file	 */
	struct point  **points,		/* -> to array of points	 */
	int	       *npoints,	/* # of points in infile  	 */
	float	       *min,		/* min data value read		 */
	float	       *max)		/* max data value read		 */
{
	int             i;
   	int		nodatacount;    /* count of nodata pts           */
	int		nread;		/* # values read in fscanf()	 */
	int		first_point;	/* flag for first pixel read	 */
	double		northing;	/* northing of point		 */
	double		easting;	/* easting of point		 */
	float 		value;		/* data value of point		 */
	struct point   *head;		/* -> to head of linked list	 */
	struct point   *tail;		/* -> to tail of linked list	 */
	struct point   *node;		/* -> to new element of list	 */


   /* initialize */

	first_point = TRUE;
	head = NULL;
	tail = NULL;
	*npoints = 0;
	nodatacount = 0;
	*min = FLT_MAX;
	*max = -1.0 * FLT_MAX;

   /* loop reading triples and copying to linked list */

	while ((nread = fscanf (ifp, "%lf%lf%f", &easting, &northing, &value))
		== 3) {


		if (value == nodataval) { nodatacount++; }

/***
fprintf(stderr,"NODATAVAL = %f\n",nodataval);
fprintf(stderr,"llist: %.0f %.0f %.0f\n", easting,northing,value);
***/

		/* allocate new node and copy data into it */

		node = (struct point *) ecalloc (1, sizeof(struct point));
		if (node == NULL) {
			error ("error allocating node for new data triple");
		}

		node->northing = northing;
		node->easting = easting;
		node->value = value;
		node->next = NULL;
		(*npoints)++;

		/* keep min and max of data */

/*
fprintf(stderr,"val, min, max: %f %f %f\n", value, *min, *max);
*/

		if (!first_point) {
			if (value != nodataval) {
			    *min = MIN (value, *min);
			    *max = MAX (value, *max);
			}
			tail->next = node;
		} else {
			if (value != nodataval) {
			    *min = value;
			    *max = value;
			}
			first_point = FALSE;
			head = node;
		}
		tail = node;
	}

	if (nread != EOF) {
		error ("error reading point %d from input file", *npoints);
	}

   /* return head of list */

	*points = head;


   /* if crossval, get arrays of coordinates */

  	if (jackflag == 1) {

	    /* allocate global point array */

	    globalpts = (PTARRAY *) ecalloc (*npoints, sizeof(PTARRAY));
	    if (globalpts == NULL) {
		error ("error allocating coordinate array");
	    }

	    /* read values as strings */

	    rewind(ifp);

	    for (i = 0; i < *npoints; i++) {

		nread = fscanf(ifp, "%s%s%s", globalpts[i].e_str,
			globalpts[i].n_str, globalpts[i].v_str);

		if (nread != 3) {
		    error ("error reading point %d from input file", i);
		}

/****&&&
if (value == nodataval) { i--; continue; }
fprintf(stderr,"array: %.0f %.0f %.0f\n", easting,northing,value);
*****/

		/* set float values in global array */

	    	globalpts[i].north = atof(globalpts[i].n_str);
	    	globalpts[i].east  = atof(globalpts[i].e_str);
	    	globalpts[i].val   = (float) atof(globalpts[i].v_str);

/***
fprintf(stderr,"point %d: e/n/val: %.0f %.0f %.0f\n", 
    i, globalpts[i].east, globalpts[i].north, globalpts[i].val); 
***/

	    } /* end for */
	}     /* end if jackflag */

	if (nodatacount > 0)
	    fprintf(stderr, "NOTE: pointfile has %d nodata %s.\n",
		nodatacount, nodatacount == 1 ? "point" : "points");
} 	      /* end function */

