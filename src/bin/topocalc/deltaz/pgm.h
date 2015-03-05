#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

int 	nbands;  /* number of output bands (1 or 2) */

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

extern void	deltaz (int fdm, int fdo, int npoints, struct point *points,
			int nclosest, double bline, double bsamp, double dline,
			double dsamp);
extern void	get_points (FILE *ifp, struct point **points, int *npoints,
			    float *min, float *max);
extern void	headers (int fdm, int fdo, float min, float max, double *bline,
			 double *bsamp, double *dline, double *dsamp, int bits);
extern double	interp (int nclosest, struct neighbor *neighbors,
			fpixel_t dem_z);
extern void	find_neighbors (double northing, double easting, int npoints,
				struct point *points, int nclosest,
				struct neighbor *neighbors);

#define DEFAULT_NPTS	12
#define DEFAULT_BITS	8
#define NODATA		-999999.0

#endif
