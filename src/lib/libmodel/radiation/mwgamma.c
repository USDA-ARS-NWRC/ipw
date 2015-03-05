#include "ipw.h"
#include "radiation.h"

void
mwgamma(
	double          u0,	/* cosine illumination angle	 	*/
	double          w,	/* single-scattering albedo	 	*/
	double          g,	/* scattering asymmetry param	 	*/
	double         *gam,	/* output gamma values		 	*/
	int             method)	/* DELTA_EDDINGTON or MEADOR_WEAVER	*/

{
	double         *g1;		/* gamma sub 1			 */
	double         *g2;		/* gamma sub 2			 */
	double         *g3;		/* gamma sub 3			 */
	double         *g4;		/* gamma sub 4			 */
	double          hd;		/* denominator			 */
	static double   b0;		/* beta sub 0			 */
	static double   mulast;		/* last value of u0		 */
	static double   glast;		/* last value of g		 */
	static bool_t   already;	/* ? initial call		 */

 /*
  * access each member of gamma vector by name
  */
	g1 = &gam[0];
	g2 = &gam[1];
	g3 = &gam[2];
	g4 = &gam[3];

	switch (method) {
	case DELTA_EDDINGTON:
/*
 * Eddington or delta-Eddington approx
 */
		if (w == 1)
			*g1 = *g2 = 3 * (1 - g) / 4;
		else {
			*g1 = (7 - (3 * g + 4) * w) / 4;
			*g2 = ((4 - 3 * g) * w - 1) / 4;
		}
		*g3 = (2 - 3 * g * u0) / 4;
		break;

	case MEADOR_WEAVER:
/*
 * Meador-Weaver hybrid approx, avoid unnecessary re-computation of beta_0
 */
		if (!already || mulast != u0 || glast != g) {
			b0 = beta_0(u0, g);
			already = 1;
			mulast = u0;
			glast = g;
		}
/*
 * hd is denom for hybrid g1,g2
 */
		hd = 4 * (1 - g * g * (1 - u0));
/*
 * (these are Horner expressions corresponding to Meador-Weaver Table 1)
 */
		if (w == 1)
			*g1 = *g2 = (g * ((3 * (g - 1) + 4 * b0) * g - 3) + 3)
				/ hd;
		else {
			*g1 = (g * (g * ((3 * g + 4 * b0) * w - 3) - 3 * w)
			       - 4 * w + 7) / hd;
			*g2 = (g * (g * ((3 * g + 4 * (b0 - 1)) * w + 1)
				    - 3 * w) + 4 * w - 1) / hd;
		}
		*g3 = b0;
		break;

	default:
 /*
  * shouldn't reach
  */
	     error("mwgamma: method = %d (not DELTA_EDDINGTON or MEADOR_WEAVOR",
		    method);
	}

	*g4 = 1 - *g3;
}
