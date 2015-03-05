#include "ipw.h"
#include "pgm.h"

double  *coslon;		/* -> table of cos(longitudes)	 */
double  *sinlon;		/* -> table of sin(longitudes)	 */
double	coslat;			/* cos(latitude)		 */
double	sinlat;			/* sin(latitude)		 */
double	cosazm;			/* cos of azimuth		 */
double	sinazm;			/* sin of azimuth		 */
double  *cosdist;		/* -> table of cos(dist)	 */
double  *sindist;		/* -> table of sin(dist)	 */

int
main(
       int     argc,
	char  **argv)
{
	static OPTION_T opt_a = {
		'a', "azimuth for horizon (deg from S, +E/-W)",
		REAL_OPTARGS, "phi",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "Sun-below-horizon mask output option"
	};

	static OPTION_T opt_s = {
		's', "solar azimuth or zenith/azimuth (if -b) image",
		STR_OPTARGS, "solar_image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_z = {
		'z', "flag for solar zenith angle in solar_image",
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_e = {
		'e', "maximum elevation",
		REAL_OPTARGS, "max_elev",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input elevation file",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_a,
		&opt_b,
		&opt_s,
		&opt_z,
		&opt_m,
		&opt_e,
		&operand,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int             fds;		/* solar image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	float		emax;		/* maximum elevation		 */
	double		blat;		/* latitude at image UL corner	 */
	double		blon;		/* longitude at image UL corner	 */
	double		dlat;		/* latitude spacing (degrees)	 */
	double		dlon;		/* longitude spacing (degrees)	 */
	double		azm;		/* azimuth for horizon (deg)	 */
	bool_t		zenith;		/* flag set if zenith in solar file	*/
	bool_t		maskout;	/* flag for mask output		 */
	bool_t		got_azm;	/* flag for azimuth specified	 */
	bool_t		got_solar;	/* flag for solar image specified*/


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (!got_opt(operand)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operand, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operand, 0));
		}
	}

	no_tty(fdi);

   /* process args */

	got_azm = got_opt (opt_a);
	if (got_azm) {
		azm = real_arg (opt_a, 0);
		if (azm < -180.0 || azm > 180.0)
			error ("Illegal azimuth: %f", azm);
	} else {
		azm = 0.0;    /* make sure it's initialized even if not used */
	}

	maskout = got_opt (opt_b);

	zenith = got_opt (opt_z);

	got_solar = got_opt (opt_s);
	if (got_solar) {
		if (got_azm)
			error ("-a and -s options are mutually exclusive");
	} else if (!got_azm) {
		error ("at least one of (-a,-s must be specified)");
	}

	if (got_opt (opt_e)) {
		emax = real_arg (opt_e, 0);
		if (emax <= 0)
			error ("Illegal maximum elevation");
	} else {
		emax = 0.0;
	}
	

   /* access solar image file */

	if (got_solar) {
		fds = uropen (str_arg(opt_s, 0));
		if (fds == ERROR) {
			error("can't open \"%s\"", str_arg(opt_s, 0));
		}
	} else {
		fds = ERROR;
	}

	no_tty(fds);

   /* access mask image file */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

	no_tty(fds);

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);
 
   /* read headers of input images/write output headers */

	headers (fdi, fds, fdm, fdo, got_solar, azm, maskout, zenith,
		 &blat, &blon, &dlat, &dlon);

   /* do it */

	ghorizon (fdi, fds, fdm, fdo, got_solar, azm, maskout, zenith, emax,
		  blat, blon, dlat, dlon);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
