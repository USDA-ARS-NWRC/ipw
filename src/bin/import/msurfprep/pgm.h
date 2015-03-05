#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define debugflag 0
#define DEFAULT_NPTS 12
#define BUFSIZE 1024
#define BYTES 2
#define BITS 16

/* degrees per radian */
#define DEG  57.2957795130823208768

/* global variables */

typedef struct {
        int             geoflag;    /* if nonzero, use geo distance alg */
} PARM_T;

extern PARM_T parm;

typedef short POINTINDEX;        /* data type for storing point indexes */
			         /* signed range must be > number of points */
typedef struct {
    double x,     		/* easting */
	   y,	  		/* northing */
           z,			/* value */
	   sinlat,		/* sin latitude */
	   coslat,		/* cos latitude */	
	   lonrad;		/* longitude in radians */
} POINT;

POINT *points; 			/* array of POINTs */

typedef struct {		/* to hold sin/cos of lines/samples */
    double sin,			/* when using spherical distance */
	   cos;		
} TRIG;

/* array of latitude cos/sin; array of longitudes in radians */
TRIG   *linetrig;
double *samplonrad;

typedef struct {
    POINTINDEX id;		/* obsolete */
    double dist2;		/* squared distance from point to cell */
} NEIB;

NEIB *neighbors; 	        /* array of neibs/distances */
NEIB *closest; 	        	/* array of closest neibs/distances */

double naflag;			/* nodata flag value */

extern void	Find_neibs(int nneibs, int npoints, double cellx, double celly,
			   int line, int samp);
extern double	geodist(double coslat1, double sinlat1, double coslat2,
			double sinlat2, double lonrad1, double lonrad2);
extern void	headers(int fdi, int fdm, int fdo, double *bline,
			double *bsamp, double *dline, double *dsamp,
			int nneibs, FILE *ptfile, int *npoints);
extern int	Read_points(FILE *ptfile, int nneibs, STRVEC_T *annotp);
extern int	get_lines(FILE *fp);
extern void	surfaceprep(int lines, int samps, int nneibs,
			    double bline, double bsamp, double dline,
			    double dsamp, int npoints, int fdo);
extern int	Compare(NEIB *elem1, NEIB *elem2);

#endif
