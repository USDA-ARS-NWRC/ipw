/*
** fix window header
*/

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

WINH_T        **
fixwinh(
	WINH_T        **i_winh)		/* -> input window header	 */
{
	WINH_T        **o_winh;		/* -> output window header	 */
	WINH_T         *i_winhp;	/* -> input WINH for a band	 */
	WINH_T         *o_winhp;	/* -> output WINH for a band	 */
	int             band;		/* band index			 */
	int             nbands;		/* # input bands		 */
	int             nlines;		/* # input lines		 */
	int             nsamps;		/* # input samples		 */

	nbands = hnbands(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * if input WINH is NULL, don't need output WINH
  */
	if (i_winh == (WINH_T **) NULL) {
		return (i_winh);
	}
 /*
  * duplicate input WINH, then make necessary changes
  */
	o_winh = winhdup(i_winh, nbands);
	assert(o_winh != NULL);

	for (band = 0; band < nbands; ++band) {
		i_winhp = i_winh[band];
		o_winhp = o_winh[band];

		if (i_winhp != (WINH_T *) NULL) {
			if (parm.lines) {
				winh_bline(o_winhp) = WIN_LINE(i_winhp,
							  nlines - 1);
				winh_dline(o_winhp) = -winh_dline(i_winhp);
			}
			if (parm.samps) {
				winh_bsamp(o_winhp) = WIN_SAMP(i_winhp,
							  nsamps - 1);
				winh_dsamp(o_winhp) = -winh_dsamp(i_winhp);
			}
		}
	}
	return (o_winh);
}
