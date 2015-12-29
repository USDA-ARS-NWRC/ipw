#ifndef	PGM_H
#define	PGM_H

#include <errno.h>

#include "../../horizon.new/IPWmacros.h"
#include "bih.h"
#include "lqh.h"

/*
 * header file for "hor1d" program
 */

typedef struct {
	bool_t          backward;	/* ? backward direction		 */
	double          azimuth;	/* azimuth (radians)		 */
	double          spacing;	/* elev grid spacing same units	 */
	double          zenith;		/* solar zenith angle (radians)	 */
	int             i_fd;		/* input image file descriptor	 */
	int             nbits;		/* # bits in output pixel	 */
	int             o_fd;		/* output image file descriptor	 */
} PARM_T;

extern PARM_T   parm;

extern bool_t   bihvalid (BIH_T **bihpp, int nb);
extern double   azmf (double azd);
extern double   zenf (double zend);
extern int	hor1b (int n, fpixel_t *z, int *h);
extern int	hor1f (int n, fpixel_t *z, int *h);
extern void	hormask (int n, fpixel_t *z, fpixel_t delta, int *h,
			 fpixel_t thresh, fpixel_t *hmask);
extern void	horval (int n, fpixel_t *z, fpixel_t delta, int *h,
			fpixel_t *hcos);
extern void     headers (void);
extern fpixel_t *hor1d (PARM_T parm, fpixel_t *zbuf);
extern LQH_T  **newlqh (int fdo);

#define NBANDS	1

#endif
