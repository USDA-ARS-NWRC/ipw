#include <math.h>
#include "ipw.h"
#include "pgm.h"

int
main (
		int             argc,
		char          **argv)
{
	static OPTION_T opt_s = {
			's', "time of last snow storm (decimal days)",
			REAL_OPTARGS, "start",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
			'd', "current day (decimal days)",
			REAL_OPTARGS, "day",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_g = {
			'g', "eff. grain radius of last storm (50-250 mu m)",
			REAL_OPTARGS, "gsize",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
			'm', "max grain radius allowed (500-1000 mu m)",
			REAL_OPTARGS, "maxgsz",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_c = {
			'c', "contamination factor for vis albedo (1.25-3.00)",
			REAL_OPTARGS, "dirt",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_v = {
			'v', "output visible band only"
	};

	static OPTION_T opt_r = {
			'r', "output ir band only"
	};

	static OPTION_T opt_i = {
			'i', "distributed time since last storm image (decimal days)",
			STR_OPERANDS, "storm image",
			OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
			OPERAND, "mu input image (defaults to stdin)",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
			&opt_s,
			&opt_d,
			&opt_g,
			&opt_m,
			&opt_c,
			&opt_v,
			&opt_r,
			&opt_i,
			&operand,
			0
	};

	int		fdi;		/* input image file desc	 */
	int		fds;		/* input image storm file desc	 */
	int		fdo;		/* output image file desc	 */
	int		obands;		/* # output bands		 */
	double		day;		/* decimal days since last storm */
	double		dirt;		/* visible contamination factor  */
	double		gsize;		/* initial grain radius 	 */
	double		maxgsz;		/* grain radius growth limit 	 */
	double		start;		/* day of last storm 		 */
	bool_t		vis_only;	/* output visible band only	 */
	bool_t		ir_only;	/* output IR band only		 */
//	bool_t		start_only;	/* use start options	 */
	bool_t		mflag;	/* use map options		 */


	/* begin */
	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	/* get/check runstring args */

	mflag = 0;
	if (got_opt (opt_i))
		mflag = 1;
	else if (got_opt (opt_s))
		start = real_arg (opt_s, 0);
	else
		error ("Must specify either -s or -i");



	day = real_arg (opt_d, 0);
	if (day < start)
		error ("day, %g, before start, %g", day, start);

	gsize = real_arg (opt_g, 0);
	if (gsize <=0 || gsize > MAX_GSIZE)
		error ("unrealistic input: gsize=%g", gsize);

	maxgsz = real_arg (opt_m, 0);
	if (maxgsz <= gsize || maxgsz > MAX_MAXGSZ)
		error ("unrealistic input: maxgsz=%g", maxgsz);

	dirt = real_arg (opt_c, 0);
	if (dirt < MIN_DIRT || dirt > MAX_DIRT)
		error ("unrealistic input: dirt=%g", dirt);

	vis_only = got_opt (opt_v);
	ir_only = got_opt (opt_r);

	if (vis_only && ir_only)
		error ("-v and -r options are mutually exclusive");

	if (vis_only || ir_only)
		obands = 1;
	else
		obands = 2;

	/* access input image */
	if (got_opt (operand)) {
		fdi = uropen (str_arg (operand, 0));
		if (fdi == ERROR)
			error ("Can't open input image %s", str_arg(operand,0));
	} else {
		fdi = ustdin();
	}

	no_tty (fdi);

	/* access storm file */
	if (!got_opt(opt_i)) {
		fds = ERROR;
	} else {
		fds = uropen (str_arg(opt_i, 0));
		if (fds == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_i, 0));
		}
		no_tty(fds);
	}

	/* access output image */
	fdo = ustdout();
	no_tty (fdo);

	/* process input/output headers */
	headers (fdi, fdo, fds, obands);

	/* read input image; calculate albedo; write output image */
	if (mflag == 1)
		ialbedo_map (fdi, fdo, fds, day, gsize, maxgsz, dirt, vis_only, ir_only);
	else
		ialbedo (fdi, fdo, start, day, gsize, maxgsz, dirt, vis_only, ir_only);



	/* all done */

	ipwexit (EXIT_SUCCESS);
}
