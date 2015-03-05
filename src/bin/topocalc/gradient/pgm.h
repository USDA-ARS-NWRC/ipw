#ifndef PGM_H
#define PGM_H

#include "geoh.h"
#include "lqh.h"
#include "IPWmacros.h"

#ifdef BIH_H
extern bool_t   bihvalid (BIH_T **bih, int nb);
#endif
extern void	caspect (int n, fpixel_t *dx, fpixel_t *dy, fpixel_t *a);
extern void	cslope (int n, fpixel_t *dx, fpixel_t *dy, fpixel_t *s);
extern void	diffxy (int n, fpixel_t *delh, fpixel_t *k0,
			fpixel_t *k1, fpixel_t *k2, fpixel_t *dx, 
			fpixel_t *dy);
extern void	fillends (int n, fpixel_t *k);
extern void	fillstart (int n, fpixel_t *k0, fpixel_t *k1,
			   fpixel_t *k2);
extern void	gradient (int fdi, int fdo, bool_t slope, bool_t aspect,
			  fpixel_t *spacing);
extern void	gradu (int nsamps, fpixel_t *delh, bool_t dos,
		       bool_t doa, fpixel_t **ibuf, fpixel_t *dx,
		       fpixel_t *dy, fpixel_t *s, fpixel_t *a,
		       fpixel_t *obuf);
extern void	headers (int fdi, int fdo, bool_t slope, bool_t aspect,
			 int *nbits, fpixel_t *spacing);
extern GEOH_T ** newgeoh (int nbands, int fdo, GEOH_T **i_geoh,
			  fpixel_t *spacing);
extern LQH_T **	newlqh (int fdo, bool_t slope, bool_t aspect);
extern void	shuffle (int n, fpixel_t *s, fpixel_t *a, fpixel_t *o);
#ifdef GETARGS_H
extern void	options (OPTION_T *opt_s, OPTION_T *opt_a,
			 OPTION_T *opt_d, OPTION_T *opt_i, bool_t *s, 
			 bool_t *a, int *nbits, fpixel_t *spacing);
#endif

#endif
