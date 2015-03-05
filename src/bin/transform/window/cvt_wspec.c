#include "ipw.h"

#include <math.h>

#include "geoh.h"
#include "winh.h"

#include "pgm.h"

/*
 * cvt_wspec -- convert extrinsic to intrinsic window spec
 */

WSPEC_T        *
cvt_wspec(
	XWSPEC_T       *xwp,		/* -> extrinsic window spec	 */
	int             nbands,		/* # image bands		 */
	WINH_T        **winhpp,		/* -> window header		 */
	GEOH_T        **geohpp)		/* -> geodetic header		 */
{
	static WSPEC_T  w;		/* intrinsic window spec	 */

	int             band;		/* header band #		 */
	double          L_div = 0;	/* line transform divisor	 */
	double          L_sub = 0;	/* line transform subtrahend	 */
	double          s_div = 0;	/* sample transform divisor	 */
	double          s_sub = 0;	/* sample transform subtrahend	 */
	double		tmp = 0;	/* scratch variable		*/

	band = xwp->band;
	if (band < 0 || band >= nbands) {
		usrerr("%d: bad band", band);
		return (NULL);
	}

	switch (xwp->flags & (GOT_XWIN | GOT_XGEO)) {
 /*
  * window spec is already intrinsic: use identity transform
  */
	case 0:
		L_sub = 0.0;
		L_div = 1.0;

		s_sub = 0.0;
		s_div = 1.0;

		break;
 /*
  * window spec is in window coordinates
  */
	case GOT_XWIN:
		if (winhpp == NULL || winhpp[band] == NULL) {
			usrerr("no window header for band %d", band);
			return (NULL);
		}

		L_sub = winh_bline(winhpp[band]);
		L_div = winh_dline(winhpp[band]);

		s_sub = winh_bsamp(winhpp[band]);
		s_div = winh_dsamp(winhpp[band]);

		break;
 /*
  * window spec is in geodetic coordinates
  */
	case GOT_XGEO:
		if (geohpp == NULL || geohpp[band] == NULL) {
			usrerr("no geodetic header for band %d", band);
			return (NULL);
		}

		L_sub = geoh_bline(geohpp[band]);
		L_div = geoh_dline(geohpp[band]);

		s_sub = geoh_bsamp(geohpp[band]);
		s_div = geoh_dsamp(geohpp[band]);

		break;
	}
 /*
  * transform the window spec
  */
	tmp = (xwp->bline - L_sub) / L_div; w.bline = rint(tmp);
	tmp = (xwp->bsamp - s_sub) / s_div; w.bsamp = rint(tmp);

	tmp = (xwp->cline - L_sub) / L_div; w.cline = rint(tmp);
	tmp = (xwp->csamp - s_sub) / s_div; w.csamp = rint(tmp);

	tmp = (xwp->eline - L_sub) / L_div; w.eline = rint(tmp);
	tmp = (xwp->esamp - s_sub) / s_div; w.esamp = rint(tmp);

	w.flags = xwp->flags & ~(GOT_XWIN | GOT_XGEO);

	w.nlines = xwp->nlines;
	w.nsamps = xwp->nsamps;

	return (&w);
}
