/*
 ** NAME
 **	init_shade -- read & write headers and allocate buffers for shade
 **
 ** DESCRIPTION
 **	Init_shade reads the input slope/azimuth header, allocates
 **	necessary buffers, and writes output header.
 **
 ** GLOBALS ACCESSED
 **	BIH of input and output files
 **	LQH of input and output files
 **	tables of sines, cosines, and cosines of differences
 **
 */

#include <math.h>

#include "ipw.h"

#include "pgm.h"

void
init_shade(
		int             fdi,		/* input file descriptor	 */
		int             fdo,		/* output file descriptor	 */
		OPTION_T       *opt_z,
		OPTION_T       *opt_u,
		OPTION_T       *opt_a)
{
	double          zen;		/* solar zenith angle (deg)	 */
	double          azm;		/* solar azimuth (deg)		 */
	double          zenith;		/* solar zenith, rad		 */
	double          azimuth;	/* solar azimuth, rad		 */
	double          ctheta;		/* cosine solar zenith		 */
	double          stheta;		/* sine solar zenith		 */

	/*
	 * process options
	 */
	azm = real_arg(*opt_a, 0);
	azimuth = azmf(azm);
	if (got_opt(*opt_u)) {
		if (got_opt(*opt_z))
			warn("both -z and -u specified, -z ignored");
		ctheta = real_arg(*opt_u, 0);
		invzenf(ctheta, &zenith, &zen);
		stheta = sin(zenith);
	}
	else if (got_opt(*opt_z)) {
		zen = real_arg(*opt_z, 0);
		zenith = zenf(zen);
		ctheta = cos(zenith);
		stheta = sin(zenith);
	}
	else {
		error("must specify either -z or -u");
	}
	/*
	 * process headers
	 */
	headers(fdi, fdo, ctheta, azimuth);
	/*
	 * allocate buffers
	 */
	buffers(fdi);
	/*
	 * values in trig tables
	 */
	trigtbl(fdi, azimuth, sintbl, costbl, cosdtbl);
	/*
	 * values in lookup table
	 */
	shadetbl(fdi, fdo, (float) ctheta, (float) stheta,
			costbl, sintbl, cosdtbl, shade);
}
