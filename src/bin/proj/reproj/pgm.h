#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#include "mapproj.h"
#include "gctp.h"

/* resampling methods */

#define NN	0
#define BI	1
#define CC      2
#define XX      3

#define NODATA  -1

extern void  headers(int fdi, int fdg, int fdm, int fdo, int o_nlines,
		     int o_nsamps, int o_proj, int o_units,
		     double *o_bline, double *o_bsamp, double *o_dline,
		     double *o_dsamp, double *i_bline, double *i_bsamp,
		     double *i_dline, double *i_dsamp);
extern void  reproj(int fdi, int fdm, int fdo, int resamp,
		    struct projdef *iproj, double i_bline,
		    double i_bsamp, double i_dline, double i_dsamp,
		    struct projdef *oproj, double o_bline,
		    double o_bsamp, double o_dline, double o_dsamp);

extern int   nearest(pixel_t **ibuf, float line, float samp,
		     int nlines, int nsamps);
extern int   bilinear(pixel_t **ibuf, float line, float samp,
		      int nlines, int nsamps);
extern int   cubic(pixel_t **ibuf, float line, float samp,
		   int nlines, int nsamps);
extern int   nothing(pixel_t **ibuf, float line, float samp,
		     int nlines, int nsamps);

#endif
