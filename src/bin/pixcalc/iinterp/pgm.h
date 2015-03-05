#include "IPWmacros.h"

/* declare all functions */

void	iinterp(int fdi, int fdo, FILE *ifp, int npoints, bool_t clip);
void	get_points(FILE *fp, int npoints, fpixel_t *xval, fpixel_t *yval);
void	headers(int fdi, int fdo, int nbits);
void	lutximg(int fdi, int nlines, int nsamps, REG_3 fpixel_t *lut,
		int fdo);
void	newlqh(int fdo, fpixel_t min, fpixel_t max);
