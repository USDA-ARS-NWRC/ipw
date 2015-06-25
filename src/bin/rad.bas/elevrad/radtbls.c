/*
 * look-up tables for beam and diffuse radiation at range of altitudes
 */

#include <math.h>
#include "ipw.h"
#include "fpio.h"

#include "pgm.h"

void
radtbls(
		int             fdi,		/* input file descriptor	 */
		double          elev,		/* elevation at tau measurement	 */
		double          mu0,		/* cosine solar zenith		 */
		double          tau,		/* optical depth at elev	 */
		double          omega,		/* single-scat albedo		 */
		double          gfact,		/* scat asym param		 */
		double          R0,		/* mean surface albedo		 */
		double          S0,		/* exoatmos solar irrad		 */
		fpixel_t       *bval,		/* min, max beam rad (returned)	 */
		fpixel_t       *dval)		/* min, max diffuse rad (ret)	 */
{
	double          btrans;		/* beam transmittance		 */
	double          gamma[4];	/* 2-stream gamma values	 */
	double          press;		/* reference pressure		 */
	double          refl;		/* reflectance			 */
	double          trans;		/* total transmittance		 */
	double          zpress;		/* pressure			 */
	fpixel_t       *zp;		/* -> elevation table		 */
	fpixel_t      **map;		/* -> look-up tables		 */
	int             j;
	int             nvals;
	int            *maplen;		/* -> lengths of all tables	 */

	/*
	 * look-up table for elevation
	 */
	map = fpmap(fdi);
	zp = map[0];
	maplen = fpmaplen(fdi);
	nvals = maplen[0];
	/*
	 * reference pressure (at reference elevation, in km)
	 */
	press = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
			elev * 1.e-3, GRAVITY, MOL_AIR);
	/*
	 * Convert each elevation in look-up table to pressure, then to
	 * optical depth.  Now look-up table automatically converts index
	 * values for elevation to optical depth!
	 */
	for (j = 0; j < nvals; ++j) {
		zpress = HYSTAT(SEA_LEVEL, STD_AIRTMP, STD_LAPSE,
				zp[j] * 1.e-3, GRAVITY, MOL_AIR);
		zp[j] = tau * zpress / press;
	}
	/*
	 * gamma values are same for all tau
	 */
	(void) mwgamma(mu0, omega, gfact, gamma, 1);
	/*
	 * allocate beam and diffuse lookup tables
	 */
	/* NOSTRICT */
	beam = (fpixel_t *) ecalloc(nvals, sizeof(fpixel_t));
	assert(beam != NULL);
	/* NOSTRICT */
	diffuse = (fpixel_t *) ecalloc(nvals, sizeof(fpixel_t));
	assert(diffuse != NULL);
	/*
	 * twostream solution for beam and diffuse look-up tables; bval and
	 * dval vectors will contain min & max values of beam and diffuse
	 * components when done
	 */
	bval[0] = dval[0] = S0;
	bval[1] = dval[1] = 0;
	for (j = 0; j < nvals; ++j) {
		if (twostream(gamma, omega, mu0,
				(double) zp[j], R0,
				&refl, &trans, &btrans) == ERROR)
			error("twostream error");
		beam[j] = S0 * btrans;
		diffuse[j] = S0 * mu0 * (trans - btrans);
		if (beam[j] > bval[1])
			bval[1] = beam[j];
		if (beam[j] < bval[0])
			bval[0] = beam[j];
		if (diffuse[j] > dval[1])
			dval[1] = diffuse[j];
		if (diffuse[j] < dval[0])
			dval[0] = diffuse[j];
	}
}
