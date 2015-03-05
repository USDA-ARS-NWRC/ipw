#ifndef	PGM_H
#define	PGM_H

#include <math.h>

#include "IPWmacros.h"

/*
 * window specification: extrinsic (window, geodetic, ...) coordinates
 */
typedef struct {
	unsigned        flags;		/* bit flags (see below)	 */
	int             band;		/* extrinsic coords band #	 */

	double          bline;		/* begin line #			 */
	double          bsamp;		/* begin sample #		 */

	double          cline;		/* center line #		 */
	double          csamp;		/* center sample #		 */

	double          eline;		/* end line #		 	 */
	double          esamp;		/* end sample #		 	 */

	int             nlines;		/* # output lines		 */
	int             nsamps;		/* # output samples		 */
} XWSPEC_T;

/*
 * window specification: intrinsic (line,sample; 0-relative) coordinates
 */
typedef struct {
	unsigned        flags;		/* bit flags (see below)	 */

	int             bline;		/* begin line #			 */
	int             bsamp;		/* begin sample #		 */

	int             cline;		/* center line #		 */
	int             csamp;		/* center sample #		 */

	int             eline;		/* end line #		 	 */
	int             esamp;		/* end sample #		 	 */

	int             nlines;		/* # output lines		 */
	int             nsamps;		/* # output samples		 */
} WSPEC_T;

#define	GOT_BEGIN	BIT(0)
#define	GOT_CENTER	BIT(1)
#define	GOT_END		BIT(2)
#define	GOT_SIZE	BIT(3)
#define	GOT_XWIN	BIT(4)
#define	GOT_XGEO	BIT(5)

#define	SCR_PREFIX	"windo"

#if defined(WIN_H) && defined(GEO_H)
extern WSPEC_T *cvt_wspec(XWSPEC_T *xwp, int nbands, WINH_T **winhpp,
			  GEOH_T **geohpp);
#endif

extern int      fix_wspec(WSPEC_T *wp, int nlines, int nsamps);
extern void     window(int i_fd, XWSPEC_T *xwp, int o_fd);

#endif
