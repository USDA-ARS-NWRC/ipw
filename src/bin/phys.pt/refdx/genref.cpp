/*
 * genref - general routine for refractive index calculations
 *
 * Interpolation is by splines.  The real part is a function of
 * log(wavelength), and log(imaginary part) is a function of
 * log(wavelength).
 *
 * Dies with error message if something wrong (such as wavelength
 * out of range).
 */

/*
 *	N_DX	# wavelengths w/o temp dependence
 */

#include	<errno.h>

COMPLEX_T *
genref(
	COMPLEX_T	*result,	/* address for results	*/
	double		lambda,		/* wavelength, microns	*/
	double		temp)		/* temperature, deg K	*/
{
	double		*mu;		/* -> wavelength table	*/
	double		*litn;		/* -> real table	*/
	double		*kappa;		/* -> imag table	*/
	int		j;

	static double	ci[3*N_DX];
	static double	cr[3*N_DX];
	static int	already = 0;

	mu = x;
	litn = &x[N_DX];
	kappa = &x[2*N_DX];

	/*
	 * On first pass, transpose, convert wavelengths and
	 * imaginary part to logs, and calculate splines
	 */
	
	if (!already) {
		already = 1;
		vtdbl(x, N_DX, 3);

		for (j = 0; j < N_DX; j++) {
			mu[j] = log(mu[j]);
			kappa[j] = log(kappa[j]);
		}
		akcoef(mu, kappa, N_DX, ci);
		akcoef(mu, litn, N_DX, cr);
	}

	/*
	 * wavelengths below temperature threshold
	 * otherwise use other routine
	 */
	
	if (lambda > WAVETHRESH) {
		result = genreft(result, lambda, temp);
	}

		/* interpolate to find value corresponding to lamda */
	else {
		errno = 0;
		if (lambda <= 0) {
			error("%s: wavelength out of range (less than zero)", FUNC);
		}
		lambda = log(lambda);
		if (errno) {
			_bug("bad log return", FUNC, __LINE__);
		}
		if (lambda < mu[0] || lambda > mu[N_DX-1]) {
			error("%s: wavelength out of range", FUNC);
		}

		errno = 0;
		result->re = spval1(lambda, mu, litn, N_DX, cr);
		result->im = exp(spval1(lambda, mu, kappa, N_DX, ci));
		if (errno) {
			if (result->im == 0.)
				errno = 0;
			else
				_bug("bad exp return", FUNC, __LINE__);
		}
	}

	return (result);
}
