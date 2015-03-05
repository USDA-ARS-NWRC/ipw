/*
**	Calculate whether tan(angle to horizon) is >= thresh.
**	If it is, sun is hidden and mask is zero, otherwise mask is non-zero
*/

#include <math.h>
#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
hormask(
	int             n,		/* length of horizon vector	 */
	fpixel_t       *z,		/* elevations			 */
	fpixel_t        delta,		/* spacing			 */
	int            *h,		/* horizon function		 */
	fpixel_t        thresh,		/* threshold			 */
	pixel_t        *hmask)		/* output mask			 */
{
	int             d;		/* difference in indices	 */
	int             i;		/* index of point		 */
	int             j;		/* index of horizon point	 */
	fpixel_t        diff;		/* elevation difference		 */

	for (i = 0; i < n; ++i) {

 /* # grid points to horizon */
		j = h[i];
		d = j - i;

 /* point is its own horizon */
		if (d == 0) {
			*hmask++ = 1;
		}

 /* else need to compare with threshold */
		else {
			if (d < 0)
				d = -d;
			diff = z[j] - z[i];
			*hmask++ = (diff / (d * delta) > thresh) ? 0 : 1;
		}
	}
}
