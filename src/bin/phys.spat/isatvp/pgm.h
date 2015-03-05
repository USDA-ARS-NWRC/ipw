#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define IBANDS	1		/* # bands in input image	 */
#define OBANDS	1		/* # bands in output image	 */

extern void 	isatvp(int fdi, int fdm, int fdo);
extern void	headers(int fdi, int fdm, int fdo, int ibands, int obands);
extern void	newlqh(int fdo, float svp_min, float svp_max);
extern void	output(char *tempfile, int fdo);

#endif
