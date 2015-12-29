/*
 * Compute the horizon for arbitrary azimuth
 * This is a wrapper for hor1d to rotate/skew/transpose the
 * elevations for input to hor1d
 *
 * 20151106 Scott Havens
 */

#include <math.h>

#include "../../transform/transpose.new/pgm.h"
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "../horizon.new/pgm.h"


//int *pointerArray(int m, int n) {
//	int i;
//
//	int *hbuf[m]; // array of pointers
//	for(i = 0; i < m; i++)
//		hbuf[i] = (int *) ecalloc(n, sizeof(int));
//
//	return hbuf;
//}

void
horizon(
		PARM_T parm,			/* parameter structure */
		fpixel_t *zbuf,			/* elevation buffer		 */
		fpixel_t *hval,		/* output buffer */
		REG_6 int samp_nbytes,	/* # bytes per sample */
		int nlines,
		int nsamps
)
{
	float phi;				/* azimuth */
	float N;				/* number of pixels */
	fpixel_t *o_buf;			/* -> output image buffer if needed */
	fpixel_t *h_buf;
	fpixel_t *s_buf;
	fpixel_t *s2_buf;
	fpixel_t *t_buf;
	int o_nlines;			/* skew output size */
	int o_nsamps;			/* skew output size */

	N = nlines * nsamps;
	phi = parm.azimuth;

	//	hf = "%auxDir/hor1d" ds dz dc " -a"
	//	hb = "%auxDir/hor1d -b" ds dz dc " -a"
	//	xp = "transpose"

	if (phi == 90) {
		// don't need to rotate image
		hor1d(parm, zbuf, hval);

	} else if (phi == -90) {
		parm.backward = TRUE;
		hor1d(parm, zbuf, hval);

	} else if (phi == 0) {
		o_buf = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
		h_buf = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));

		ximg(zbuf, nlines, nsamps, o_buf);	// transpose
		hor1d(parm, o_buf, h_buf);			// calculate horizon
		ximg(h_buf, nlines, nsamps, hval);	// transpose back

	} else if (abs(phi) == 180) {
		// -180/180 are the same angle and do the same thing
		parm.backward = TRUE;

		o_buf = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
		h_buf = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));

		ximg(zbuf, nlines, nsamps, o_buf);	// transpose
		hor1d(parm, o_buf, h_buf);			// calculate horizon
		ximg(h_buf, nlines, nsamps, hval);	// transpose back

	} else if (-45 <= phi && phi <= 45) {

		skew(zbuf, nlines, nsamps, 1, TRUE, phi, 1, &o_nsamps, &s_buf);

		o_buf = (fpixel_t *) ecalloc(nlines * o_nsamps, sizeof(fpixel_t));
		h_buf = (fpixel_t *) ecalloc(nlines * o_nsamps, sizeof(fpixel_t));
		t_buf = (fpixel_t *) ecalloc(nlines * o_nsamps, sizeof(fpixel_t));
//		s2_buf = (fpixel_t *) ecalloc(nlines * o_nsamps, sizeof(fpixel_t));

		ximg(s_buf, nlines, o_nsamps, o_buf);	// transpose
		hor1d(parm, o_buf, h_buf);			// calculate horizon

		ximg(h_buf, o_nsamps, nlines, t_buf);	// transpose
		skew(t_buf, nlines, o_nsamps, nsamps, 1, TRUE, phi, 1, &s2_buf);

		//		print skew, phi, image, "|", xp, "|", hf, phi, "|", xp, "|", unskew
	} else if (-135 >= phi && phi > -180) {
		//		print skew, phi+180, image, "|", xp, "|", hb, phi+180, "|", xp, "|", unskew
	} else if (180 > phi && phi >= 135) {
		//		print skew, phi-180, image, "|", xp, "|", hb, phi-180, "|", xp, "|", unskew
	} else if (45 < phi && phi < 135) {
		//		print xp, image, "|", skew, 90-phi, "|", xp, "|", hf, phi, "|", xp, "|", unskew, "|", xp
	} else if (-45 > phi && phi > -135) {
		//		print xp, image, "|", skew, -90-phi, "|", xp, "|", hb, phi+180, "|", xp, "|", unskew, "|", xp
	} else {
		error( "error: angle =", phi);
	}



}
