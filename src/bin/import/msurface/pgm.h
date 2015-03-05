#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

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

extern void	get_distance(double cellx, double celly, int npoints,
			     int nclosest);
extern void	find_neibs(int nclosest, int npoints, double cellx,
			   double celly);
extern int	get_points(FILE *ptfile, int *npoints, float *min, float *max);
extern int	Get_items(FILE *fp);
extern void	headers(int fdi, int fdm, int fdo, float min, float max,
			double *bline, double *bsamp, double *dline,
			double *dsamp, int nclosest, int obits,
			int *inbands, FILE *ptfile, int npoints);
extern double	interp(int nclosest);
extern void	surface(int fdi, int fdm, int fdo, int npoints, int nclosest,
			double bline, double bsamp, double dline, double dsamp,
			int inbands);
extern void	dump(int n, int line, int when);
extern double	geodist(double lat1, double lon1, double lat2, double lon2);

#endif
