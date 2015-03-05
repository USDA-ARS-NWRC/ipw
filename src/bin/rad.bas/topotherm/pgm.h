#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define IBANDS	4		/* # bands in input image	 */
#define OBANDS	1		/* # bands in output image	 */

extern void 	topotherm(int fdi, int fdm, int fdv, int fdc, int fdo);
extern void	headers(int fdi, int fdm, int fdv, int fdc, int fdo,
			int ibands, int obands);
extern void	newlqh(int fdo, float lw_min, float lw_max);
extern void	output(char *tempfile, int fdo);

#endif
