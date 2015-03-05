#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "envphys.h"

#define	IBANDS		4	/* # bands in input image	 */
#define	OBANDS		2	/* # bands in output image	 */

#define NO_DATA		-99999.0

void	headers(int fdi, int fdm, int fdo, int *nbits);
void	newlqh(int fdo, double *bval, double *dval);
void	gelevrad(int fdi, int fdm, int fdo, double zelev, double omega,
		 double gfact, double S0);
void	output(char *tempfile, int fdo);

#endif
