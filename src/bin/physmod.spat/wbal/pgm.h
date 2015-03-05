#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define IBANDS	5		/* # bands in input image	 */
#define OBANDS	3		/* # bands in output image	 */

extern void 	wbal(int fdi, int fdm, int fdo);
extern void	headers(int fdi, int fdm, int fdo, int ibands, int obands);
extern void	newlqh(int fdo, float ro_min, float ro_max, float s_min,
		       float s_max, float et_min, float et_max);
extern void	output(char *tempfile, int fdo);
extern float	baseflow(float bflowf, float ssat);
extern float	aet(float pet, float ssat);

#endif
