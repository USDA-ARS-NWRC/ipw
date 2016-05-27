#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "lqh.h"

#define IBANDS		2	/* # bands in input image	*/
#define OBANDS 		1	/* # bands in output image	*/

#define RH_MIN		0.0	/* minimum RH for LQ header	*/
#define RH_MAX		1.0	/* maximum RH for LQ header	*/
#define RH_ERR		0.995	/* Max % saturation for data	*/
#define VP_MIN		0.0	/* minimum VP for LQ header	*/
#define VP_MAX		7500.0	/* maximum VP for LQ header	*/

extern void	headers(int fdi, int fdm, int nbits, int fdo);
extern void	irh2vp(int fdi, int fdm, bool_t inverse, bool_t clip, int fdo);
extern void	newlqh(int fdo, bool_t inverse, double min, double max);
extern void	output(char *tempfile, int fdo);
extern double	sati();

#endif
