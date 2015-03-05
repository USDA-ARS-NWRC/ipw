#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define DEFAULT_NPTS	12
#define NODATA		-9999.0
#define STRMAX		80

double  nodataval;	 /* on default, gets value of NODATA */

int   jackflag;  	/* if nonzero, do crossvalidation */
char *jackfile;		/* name of ascii output file */

typedef struct {
	char     n_str[STRMAX];
	char     e_str[STRMAX];
	char     v_str[STRMAX];
	double	 north;
	double	 east;
	float 	 val;
} PTARRAY;

PTARRAY *globalpts;  	/* array of point structures for crossval */


struct point {
	double	northing;
	double	easting;
	double 	value;
	struct  point *next;
};

struct neighbor {
	double dist2;
	double value;
};

extern void	get_points(FILE *ifp, struct point **points, int *npoints,
			   float *min, float *max);
extern void	headers(int fdi, int fdm, int fdo, float min, float max,
			double *bline, double *bsamp, double *dline,
			double *dsamp);
extern void	crossval(int fdm, int fdo, int npoints, struct point *points,
			 int nclosest, double bline, double bsamp,
			 double dline, double dsamp);
extern double	interp(int nclosest, struct neighbor *neighbors);
extern void	find_neighbors(double northing, double easting, int npoints,
			       struct point *points, int nclosest,
			       struct neighbor *neighbors, float val);

#endif
