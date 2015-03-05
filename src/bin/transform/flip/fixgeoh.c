/*
** fix geodetic header
*/

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

GEOH_T        **
fixgeoh(
	GEOH_T        **i_geoh)		/* -> input geodetic header	 */
{
	GEOH_T        **o_geoh;		/* -> output geodetic header	 */
	GEOH_T         *i_geohp;	/* -> input GEOH for a band	 */
	GEOH_T         *o_geohp;	/* -> output GEOH for a band	 */
	int             band;		/* band index			 */
	int             nbands;		/* # input bands		 */
	int             nlines;		/* # input lines		 */
	int             nsamps;		/* # input samples		 */

	nbands = hnbands(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * if input GEOH is NULL, don't need output GEOH
  */
	if (i_geoh == (GEOH_T **) NULL) {
		return (i_geoh);
	}
 /*
  * duplicate input GEOH, then make necessary changes
  */
	o_geoh = geohdup(i_geoh, nbands);
	assert(o_geoh != NULL);

	for (band = 0; band < nbands; ++band) {
		i_geohp = i_geoh[band];
		o_geohp = o_geoh[band];

		if (i_geohp != (GEOH_T *) NULL) {
			if (parm.lines) {
				geoh_bline(o_geohp) = GEO_LINE(i_geohp,
							  nlines - 1);
				geoh_dline(o_geohp) = -geoh_dline(i_geohp);
			}
			if (parm.samps) {
				geoh_bsamp(o_geohp) = GEO_SAMP(i_geohp,
							  nsamps - 1);
				geoh_dsamp(o_geohp) = -geoh_dsamp(i_geohp);
			}
		}
	}
	return (o_geoh);
}
