#include <math.h>
#include <strings.h>

#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "pixio.h"
#include "skewh.h"

#include "pgm.h"

/*
 * skew -- image horizontal skew/deskew
 */

double *skew(
		int             i_fd,		/* input image file descriptor	 */
		bool_t          fwd,		/* ? forward : inverse skew	 */
		double          angle,		/* skew angle			 */
		int             o_fd)		/* output image file descriptor	 */
{

	double *w;
	w = 2.0;
	return w;

}
