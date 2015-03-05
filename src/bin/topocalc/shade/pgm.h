#ifndef PGM_H
#define PGM_H
 
#include "IPWmacros.h"

/*
 * variables passed externally between
 *	init_shade()
 */

#include "ipw.h"
#include "fpio.h"
#include "pixio.h"
#include "sunh.h"
#include "lqh.h"

extern pixel_t **shade;		/* store already computed values */
extern pixel_t  *obuf;		/* binary fraction output buffer */
extern float    *cosdtbl;	/* all values of cos(phi-A)	 */
extern float    *costbl;	/* all possible values of cosS	 */
extern fpixel_t *sintbl;	/* all possible values of sinS	 */
extern int       nget;		/* # pixels to read		 */
extern pixel_t  *ibuf;		/* binary frac slope/azm buffers */

extern double	azmf(double azd);
extern void	buffers(int fdi);
extern void	cosines(int fdi, int fdo);
extern void	headers(int fdi, int fdo, double ctheta, double azimuth);
extern void	init_shade(int fdi, int fdo, OPTION_T *opt_z, OPTION_T *opt_u,
			   OPTION_T *opt_a);
extern void	invzenf(double u0, double *zenith, double *zend);
extern LQH_T  **newlqh(int fdi, int fdo);
extern SUNH_T **newsunh(int fdo, double czen, double azm);
extern void	shadetbl(int fdi, int fdo, FREG_2 float ctheta,
			 FREG_3 float stheta, REG_3 float *sintbl,
			 REG_4 float *costbl, REG_5 float *cosdtbl,
			 REG_6 pixel_t **shade);
extern void	trigtbl(int fdi, double phi, fpixel_t *sintbl, float *costbl,
			float *cosdtbl);
extern double	zenf(double zend);

#endif
