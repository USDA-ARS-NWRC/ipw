#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define DEFAULT_NPTS	12
#define NODATA		-999999.0

struct point {
	double	northing;
	double	easting;
	double 	value;
	struct point *next;
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
extern void	surface(int d1, int fdm, int fdo, int npoints,
			struct point *points, int nclosest, double bline,
			double bsamp, double dline, double dsamp);
extern double	interp(int d1, int nclosest, struct neighbor *neighbors);
extern void	find_neighbors(double northing, double easting, int npoints,
			       struct point *points, int nclosest,
			       struct neighbor *neighbors);

#endif
