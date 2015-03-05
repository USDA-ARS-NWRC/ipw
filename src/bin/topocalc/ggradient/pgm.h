#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "geoh.h"
#include "lqh.h"

extern void     fillends (int n, fpixel_t *k);
extern void     fillstart (int n, fpixel_t *k0, fpixel_t *k1, fpixel_t *k2);
extern void     gcp_dist (double lat1, double lon1, double lat2,
			  double lon2, double *distance, double *azm);
extern void     ggradient (int fdi, int fdm, int fdo, bool_t slope,
			   bool_t aspect, double blat, double blon, double dlat,
			   double dlon);
extern void     gradu (int nsamps, double lat_dist, double *lon_dist,
		       double dlat, double dlon, bool_t dos, bool_t doa, 
		       fpixel_t **ibuf, fpixel_t *slope, fpixel_t *aspect,
		       int fdm, pixel_t *mbuf, fpixel_t *obuf);
extern void     headers (int fdi, int fdm, int fdo, bool_t slope,
			 bool_t aspect, int *nbits, double *blat, double *blon,
			 double *dlat, double *dlon);
extern LQH_T ** newlqh (int fdo, bool_t slope, bool_t aspect);
extern void     shuffle (int n, fpixel_t *s, fpixel_t *a, fpixel_t *o);

#endif
