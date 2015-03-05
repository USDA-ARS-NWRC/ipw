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
*/

#include "ipw.h"
#include "pgm.h"

void
get_points (
	FILE	       *ifp,		/* file ptr to input file	 */
	struct point  **points,		/* -> to array of points	 */
	int	       *npoints,	/* # neighbors in interp	 */
	float	       *min,		/* min data value read		 */
	float	       *max)		/* max data value read		 */
{
	int		nread;		/* # values read in fscanf()	 */
	int		first_point;	/* flag for first pixel read	 */
	double		northing;	/* northing of point		 */
	double		easting;	/* easting of point		 */
	float		value;		/* data value of point		 */
	struct point   *head;		/* -> to head of linked list	 */
	struct point   *tail;		/* -> to tail of linked list	 */
	struct point   *node;		/* -> to new element of list	 */


   /* initialize */

	first_point = TRUE;
	head = NULL;
	tail = NULL;
	*npoints = 0;

   /* loop reading triples and copying to linked list */

	while ((nread = fscanf (ifp, "%lf%lf%f", &easting, &northing, &value))
		== 3) {

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

		if (!first_point) {
			*min = MIN (value, *min);
			*max = MAX (value, *max);
			tail->next = node;
		} else {
			*min = value;
			*max = value;
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
}
