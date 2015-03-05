#include "ipw.h"
#include "pgm.h"
#include "solar.h"

/*
** NAME
**	solar -- exoatmospheric solar irradiance
**
** SYNOPSIS
**	void
**	solar(
**		int             n,		|* # values in range	 *|
**		double         *range,		|* range of wavelengths	 *|
**		int            *date,		|* yr, mo, day		 *|
**		bool_t          abbrev)		|* ? abbreviated output	 *|
**
** DESCRIPTION
**	Solar prints values of exoatmospheric solar irradiance.  The
**	vector range of length 2 contains the range over which
**	irradiance is to be integrated.  If n is 1, then spectral
**	irradiance at range[0] is calculated.  If range is NULL,
**	monochromatic values are calculated for wavelengths read from
**	stdin.  The vector date contains yr, mo, day.  If not NULL, the
**	radius vector for a specific date is included in the
**	calculations.  Otherwise, a radius vector of 1.0 is used.
**
*/

void
solar(
	int             n,		/* # values in range	 */
	double         *range,		/* range of wavelengths	 */
	int            *date,		/* yr, mo, day		 */
	bool_t          abbrev)		/* ? abbreviated output	 */
{
	double          rvsq;		/* (radius vector)^2		 */
	double          declin;		/* declination (not used)	 */
	double          omega;		/* solar longitude (not used)	 */
	double          wave;		/* wavelength (um)		 */
	bool_t          tty;		/* ? input from tty		 */
	datetime_t     *dt;

 /*
  * if specific date, calculate radius vector
  */
	if (date != NULL) {
		dt = make_dt(date[0], date[1], date[2], 12, 0, 0);
		if (dt == NULL)
			error("invalid date");
		if (ephemeris(dt, &rvsq, &declin, &omega) == ERROR) {
			error("ephemeris error");
		}
		rvsq *= rvsq;
		SAFE_FREE(dt);
	}
	else {
		rvsq = 1;
	}
 /*
  * integral over a wavelength range, or specific wavelengths
  */
	if (range != NULL) {
		switch (n) {
		case 2:
			printf("%g", solint(range[0], range[1]) / rvsq);
			if (!abbrev)
				printf(" W m^-2");
			putchar('\n');
			break;
		case 1:
			printf("%g", solval(range[0]) / rvsq);
			if (!abbrev)
				printf(" W m^-2 um^-1");
			putchar('\n');
			break;
		default:
			bug("n != 1  and  n != 2");
		}
	}
	else {
		if ((tty = isatty(fileno(stdin))))
			fprintf(stderr, "enter wavelengths from stdin\n");
		fprintf(stderr, "\nvalues in W m^-2 um^-1\n");
		while (scanf("%lf", &wave) == 1) {
			if (!tty)
				printf("%g\t", wave);
			printf("%g\n", solval(wave) / rvsq);
		}
	}
}
