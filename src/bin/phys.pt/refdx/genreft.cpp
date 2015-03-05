/*
 * genreft - temperature-dependent refractive index calculations
 *
 * Interpolation is by splines.  The real part is a function of
 * log(wavelength), and log(imaginary part) is a function of
 * log(wavelength).  Temperature dependence is linear.
 *
 * Dies with error message if something wrong (such as wavelength
 * out of range).
 */

/*
 *	N_DXTT	# wavelengths w temp dependence
 */

#include	<errno.h>

COMPLEX_T *
genreft(
	COMPLEX_T	*result,
	double		lambda,		/* wavelength, microns	*/
	double		temp)		/* temperature, deg C	*/
{
	double		*mu;		/* -> wavelength table	*/
	double		*kappa[N_TEMP];	/* -> imag part		*/
	double		*litn[N_TEMP];	/* -> real part		*/
	double		imagpart[N_TEMP];
	double		realpart[N_TEMP];
	double		timag[3*N_TEMP];
	double		treal[3*N_TEMP];
	int		j;
	int		k;

	static double	ci[N_TEMP][3*N_DXT];
	static double	cr[N_TEMP][3*N_DXT];
	static int	already = 0;

	/* -> wavelengths */
	mu = xt;

	/* -> real and imag indices */
	for (k = j = 0; j < N_TEMP; j++) {
		litn[j] = &xt[++k * N_DXT];
		kappa[j] = &xt[++k * N_DXT];
	}

	/*
	 * On first pass, transpose, convert wavelengths and
	 * imaginary part to logs, and calculate splines
	 */
	
	if (!already) {
		already = 1;
		vtdbl(xt, N_DXT, 2*N_TEMP+1);

		for (j = 0; j < N_DXT; j++) {
			mu[j] = log(mu[j]);
			for (k = 0; k < N_TEMP; k++)
				kappa[k][j] = log(kappa[k][j]);
		}

		for (k = 0; k < N_TEMP; k++) {
			akcoef(mu, kappa[k], N_DXT, ci[k]);
			akcoef(mu, litn[k], N_DXT, cr[k]);
		}
	}

	/* interpolate to find values corresponding to lamda */
	errno = 0;
	lambda = log(lambda);
	if (errno) {
		_bug("bad log return", FUNCT, __LINE__);
	}

	/* wavelength and temperature within range? */
	if (lambda < mu[0] || lambda > mu[N_DXT-1]) {
		error("%s: wavelength out of range", FUNCT);
	}
	if (temp < tval[0] || temp > tval[N_TEMP-1]) {
		error("%s: temperature out of range", FUNCT);
	}

	/* values for all temperatures at this wavelength */
	for (k = 0; k < N_TEMP; k++) {
		realpart[k] = spval1(lambda, mu, litn[k], N_DXT, cr[k]);
		imagpart[k] = spval1(lambda, mu, kappa[k], N_DXT, ci[k]);
	}

	/* interpolation coefficients at this wavelength */
	akcoef(tval, imagpart, N_TEMP, timag);
	akcoef(tval, realpart, N_TEMP, treal);

	/* now interpolate for temperature */
	errno = 0;
	result->re = spval1(temp, tval, realpart, N_TEMP, treal);
	result->im = exp(spval1(temp, tval, imagpart, N_TEMP, timag));
	if (errno) {
		if (result->im == 0.)
			errno = 0;
		else
			_bug("bad exp return", FUNCT, __LINE__);
	}

	return (result);
}
