#ifndef PGM_H
#define PGM_H

/**
** Header file for trbxfr model
**/

#include "IPWmacros.h"

#include <stdio.h>
#include "lqh.h"
#include "geoh.h"

#define IBANDS		5	/* # bands in input image	*/
#define OBANDS 		3	/* # bands in output image	*/

#define HRS_2_SEC	3600.0
#define NO_DATA		-9999999.0	/* no data value - lower than mins */

#define Z_DEFAULT	3.0		/* default (meters) for upper height */

extern int	get_sample(int fdi, fpixel_t *inbuf, pixel_t *mbuf, int samp,
			   int *buf_index, double *vector, int nbands);
extern void	headers(int fdi, int fdm, int fdo, int ibands, int obands);
extern int	hle1(double press, double ta, double ts, double za,
		     double ea, double es, double zq, double u, double zu,
		     double z0, double *h, double *le, double *e);
extern void	itrbxfr(int fdi, int fdm, int fdo, double delta_t,
			double z);
extern void	newlqh(int fdo, double h_min, double h_max, double le_min,
		       double le_max, double mm_min, double mm_max);
extern void	output(char *tempfile, int fdo);

#endif
