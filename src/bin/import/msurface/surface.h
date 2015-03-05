
/*	SCCS version: @(#)   surface.h   1.1   7/19/91	*/

#ifndef SURFACE_H
#define SURFACE_H

#define DEFAULT_NPTS	12
#define NODATA		-9999.0
#define BUFSIZE		1024	

/* global variables */

/*** todo:  put all globals into the parm structure */

int nlines;
int nsamps;
double naflag;  /* nodata flag value */


typedef struct {
        int             geoflag;    /* if nonzero, use geo distance alg */
} PARM_T;

extern PARM_T parm;


typedef short POINTINDEX;

typedef struct {
	double	x;
	double	y;
	double 	z;
} POINT;

typedef struct {
	double dist2;
	double value;
} NEIB;

POINT          *points;         /* array of POINTs */
NEIB           *neighbors;      /* array of NEIBs within bound.box */
POINTINDEX     **matrix;     	/* matrix of dim (lines*samps) by nneibs */
pixel_t	      *close_id;	/* array of closest neighbor indexes */


POINTINDEX **Get_matrix();
int	get_points();
void	headers();
void	surface();
double	interp();
void	find_neighbors();


#endif
