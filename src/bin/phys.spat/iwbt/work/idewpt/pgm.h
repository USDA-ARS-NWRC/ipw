#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define IBANDS	1		/* # bands in input image	 */
#define OBANDS	1		/* # bands in output image	 */

extern void 	idewpt(int fdi, int fdm, int fdo);
extern void	headers(int fdi, int fdm, int fdo, int ibands, int obands);
extern void	newlqh(int fdo, float dpt_min, float dpt_max);
extern void	output(char *tempfile, int fdo);

extern double  dew_point(double ea);

#endif
