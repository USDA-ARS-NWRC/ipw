/*
**	Calculate values of cosines of angles to horizons, measured
**	from zenith, from elevation difference and distance.  Let
**	G be the horizon angle from horizontal and note that:
**
**		sin G = z / sqrt( z^2 + dis^2);
**
**	This result is the same as cos H, where H measured from zenith.
*/

#include <math.h>
#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
horval(
	int             n,		/* length of horizon vector	 */
	fpixel_t       *z,		/* elevations			 */
	fpixel_t        delta,		/* spacing			 */
	int            *h,		/* horizon function		 */
	fpixel_t       *hcos)		/* cosines of angles to horizon	 */
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
			*hcos++ = 0;
		}

 /* else need to calculate sine */
		else {
			if (d < 0)
				d = -d;
			diff = z[j] - z[i];
			*hcos++ = diff / (fpixel_t) hypot(diff, d * delta);
		}
	}
}
