/* refh2o - refractive index of water */

#include <math.h>

#include "ipw.h"
#include "pgm.h"

#include "waterdata.h"

#define genref		refh2o
#define genreft		trefh2o		/* dummy */

#define FUNC	"refh2o"
#define FUNCT	"trefh2o"

#include "genref.cpp"

/*
 * Refractive index of water for for 0.1 - 100 cm :
 *
 * Ray, P., 1972,  Broadband complex refractive indices of ice and
 * water, Appl. Opt. 11, 1836-1844
 */

#define ab(wl, bet, wlcen, del, gam)	( (bet) * \
	exp( -pow( fabs( log10((wl)/(wlcen)) / (del)), gam)))

#define WAVELIMIT 1.e+6
#define FREEZE 273.15

COMPLEX_T *
trefh2o(
	COMPLEX_T	*refwat,
	double		lambda,		/* wavelength, microns	*/
	double		tc)		/* temperature, deg C	*/
{
	double		denom;
	double		epsils;	/* static dielectric const Ray eq 4	*/
	double		epsim;	/* imag part of dielectric const	*/
	double		pio2;	/* pi / 2				*/
	double		power;
	double		temp;	/* temperature deg K			*/
	double		wlcm;	/* wavelength in cm			*/
	double		wls;	/* relaxation wavelength, cm		*/
	static double	alpha;
	static double	cs;
	static double	diff;	/* (epsils - epsinf)			*/
	static double	epsinf;	/* high freq dielectric const Ray eq 7a	*/
	static double	epsre;	/* real part of dielectric const	*/
	static double	sn;
	static double	tlast;	/* temperature of previous call		*/
	static int	already;	/* called previously?		*/

	if (lambda > WAVELIMIT) {
		error("wavelength %e out of range", lambda);
	}
	wls = 0;

	/*
	 * check against temperature of previous call
	 */
	if (tc != tlast || !already) {

		/* Ray's fits to Debye expressions */
		epsinf = 5.27137 + tc * (0.0216474 - 0.00131198 * tc);
		epsils = 78.54 * (1. + (tc - 25. ) * (-4.579e-3 +
			(tc - 25. ) * (1.19e-5 - 2.8e-8 * (tc - 25.))));
		diff = epsils - epsinf;

		/* temperature in Kelvin */
		temp = tc + FREEZE;

		/* Ray eq. 7b */
		pio2 = 2.0 * atan(1.0);
		alpha = -16.8129 / temp + 0.0609265;
		alpha *= pio2;
		sn = sin(alpha);
		cs = cos(alpha);
		wls = 3.3836e-4 * exp (2514. / temp);
		already = 1;
		tlast = tc;
	}

	wlcm = 1.e-4 * lambda;
	power = pow(wls / wlcm , 1. - alpha);
	denom = 1. + 2. * power * sn + power*power;
	epsre = epsinf + diff * (1. + power * sn) / denom;
	epsim = diff * power * cs / denom + 0.00666667 * wlcm;

	/*
	 * complex refractive index from Cole-Cole exten. to Debye theory
	 * refwat = sqrt(epsre + i * epsim);
	 */
	
	refwat->re = refwat->im = hypot(epsim, epsre);
	refwat->re += epsre;
	refwat->im -= epsre;
	refwat->re /= 2;
	refwat->im /= 2;
	refwat->re = sqrt(refwat->re);
	refwat->im = sqrt(refwat->im);
 
	if (lambda < 3000.) {
		/*
		 * in 1000-3000 micron region, add
		 * corrections to imag. index to
		 * account for absorption bands
		 * (Ray eq. 8 + table 2)
		 */
		refwat->im += ab(lambda, 0.25, 300.,0.47, 3.0)
			+ ab(lambda, 0.39, 17., 0.45, 1.3)
			+ ab(lambda, 0.41, 62., 0.35, 1.7);
	}

	return (refwat);
}
