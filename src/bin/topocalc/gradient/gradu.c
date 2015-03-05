/*
**	This is where the work is done.  Compute derivatives, slopes and
**	apsects, and shuffle.
*/

#include "ipw.h"

#include "pgm.h"

void
gradu(
	int             nsamps,		/* # samples in line		 */
	fpixel_t       *delh,		/* grid spacing (1-line 2-samp)	 */
	bool_t          dos,		/* ? compute slope		 */
	bool_t          doa,		/* ? compute aspect		 */
	fpixel_t      **ibuf,		/* input buffers		 */
	fpixel_t       *dx,		/* partial w.r.t. x		 */
	fpixel_t       *dy,		/* partial w.r.t. y		 */
	fpixel_t       *s,		/* sinS				 */
	fpixel_t       *a,		/* aspect			 */
	fpixel_t       *obuf)		/* output buffer		 */
{
 /*
  * end pixels of line 2
  */
	fillends(nsamps, ibuf[2]);

 /*
  * finite differences
  */
	diffxy(nsamps, delh, ibuf[0], ibuf[1], ibuf[2], dx, dy);

 /*
  * compute sinS
  */
	if (dos) {
		cslope(nsamps, dx, dy, s);
	}

 /*
  * compute A
  */
	if (doa) {
		caspect(nsamps, dx, dy, a);
	}

 /*
  * shuffle into output buffer if both slopes and aspects
  */

	if (dos && doa) {
		shuffle(nsamps, s, a, obuf);
	}
}
