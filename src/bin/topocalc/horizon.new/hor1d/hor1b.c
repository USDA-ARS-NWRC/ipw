/*
 * horizon in backward direction for equi-spaced elevation vector
 */

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"
#include "fpio.h"


#define SLOPE(i,j,zi,zj)	\
		( ((zj) <= (zi))? 0 : ((zj)-(zi)) / ((float)((i)-(j))) )

int
hor1b(
		REG_6 int       n,		/* length of vectors b and h	 */
		REG_4 fpixel_t *z,		/* elevation function		 */
		REG_1 int      *h)		/* horizon function		 */
{
	FREG_2 fpixel_t sihj;		/* slope i to h[j]		 */
	FREG_1 fpixel_t sij;		/* slope i to j			 */
	FREG_3 fpixel_t zi;		/* z[i]				 */
	REG_5 int       i;		/* point index			 */
	REG_3 int       j;		/* point index			 */
	REG_2 int       k;		/* h[j]				 */

	/*
	 * end point is its own horizon in forward direction; first point is
	 * its own horizon in backward direction
	 */
	h[0] = 0;

	/*
	 * For forward direction, loop runs from next-to-end backward to
	 * beginning.  For backward direction, loop runs from
	 * next-to-beginning forward to end.
	 */
	for (i = 1; i < n; ++i) {
		zi = z[i];

		/*
		 * Start with next-to-adjacent point in either forward or backward
		 * direction, depending on which way loop is running. Note that we
		 * don't consider the adjacent point; this seems to help reduce noise.
		 */
		if ((k = i - 2) < 0)
			++k;
		/*
		 * loop until horizon found
		 */

		do {
			/*
			 * slopes from i to j and from j to its horizon
			 */

			j = k;
			k = h[j];
			sij = SLOPE(i, j, zi, z[j]);
			sihj = SLOPE(i, k, zi, z[k]);

			/*
			 * if slope(i,j) >= slope(i,h[j]), horizon has been found; otherwise
			 * set j to k (=h[j]) and loop again
			 */

		} while (sij < sihj);

		/*
		 * if slope(i,j) > slope(j,h[j]), j is i's horizon; else if slope(i,j)
		 * is zero, i is its own horizon; otherwise slope(i,j) = slope(i,h[j])
		 * so h[j] is i's horizon
		 */

		if (sij > sihj) {
			h[i] = j;
		}
		else if (sij == 0) {
			h[i] = i;
		}
		else {
			h[i] = k;
		}
	}
	return (0);
}
