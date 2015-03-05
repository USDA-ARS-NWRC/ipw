#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define	IBANDS		3		/* # bands in input image	 */
#define	OBANDS		1		/* # bands in output image	 */

#define NO_DATA		-9999999.0	/* no data value - lower than min */


extern void	headers(int fdi, int fdm, int fdo);
extern int	get_sample(int fdi, fpixel_t *inbuf, pixel_t *mbuf,
			   double *vector, int nbands);
extern void	iptx(int fdi, int fdm, int fdo);
extern void	newlqh(int fdo, double le_min, double le_max);
extern void	output(char *tempfile, int fdo);

#endif
