#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "lqh.h"

#define IBANDS		3	/* # bands in input image	*/
#define OBANDS 		1	/* # bands in output image	*/

extern void	headers(int fdi, int fdm, int nbits, int fdo);
extern void	iwbt(int fdi, int fdm, int fdo);
extern void	newlqh(int fdo, double min, double max);
extern void	output(char *tempfile, int fdo);
extern double	sati();
extern double	wetbulb();

#endif
