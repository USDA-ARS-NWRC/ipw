/*
 *  Global variables and routines that are internal to the sunang program.
 */

#include "IPWmacros.h"

/*
 *  Routines
 */

extern void   sunang_opts(OPTION_T *b, OPTION_T *l, OPTION_T *s,
		          OPTION_T *a, OPTION_T *r, double *lat,
		          double *lon, double *slope, double *azm);
extern int    sunang(datetime_t * datetime, double latitude, double longitude,
		     int locGradient, double slopeCos, double azmSlope,
		     double *mu, double *azmSun, double *radVec);
