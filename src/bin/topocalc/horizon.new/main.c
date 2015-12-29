#include <math.h>
#include <errno.h>

#include "../horizon.new/pgm.h"
#include "ipw.h"
#include "bih.h"

int
main(
		int             argc,
		char          **argv)
{
	static OPTION_T opt_a = {
			'a', "direction of forward azimuth (degrees)",
			REAL_OPTARGS, "phi",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_d = {
			'd', "grid spacing (normally obtained from GEOH header)",
			REAL_OPTARGS, "delta",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
			'b', "backward direction (default: forward)"
	};

	static OPTION_T opt_z = {
			'z', "mask option, arg is solar zenith angle (degrees) or",
			REAL_OPTARGS, "zen",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
			'u', "mask option, arg is cosZ",
			REAL_OPTARGS, "cos",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
			'n', "# bits / output pixel (default: 8)",
			INT_OPTARGS, "#bits",
			OPTIONAL, 1, 1
	};

	static OPTION_T efile = {
			OPERAND, "input elevation image",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
			&opt_a,
			&opt_b,
			&opt_z,
			&opt_u,
			&opt_d,
			&opt_n,
			&efile,
			0
	};

	fpixel_t	*zbuf;			/* elevation buffer		 */
	int			N;				/* # pixels in row		 */
	//	fpixel_t	*hval;			/* return from hor1d */
	int 		i;				/* loop index */
	int 		nsamps;			/* number of samples */
	int 		nlines;			/* number of lines */
	REG_6 int	samp_nbytes;	/* # bytes per sample		 */

	/*
	 * begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 * azimuth
	 */
	parm.azimuth = real_arg(opt_a, 0);

	/*
	 * spacing
	 */
	if (got_opt(opt_d)) {
		parm.spacing = real_arg(opt_d, 0);
		if (parm.spacing <= 0)
			error("-d %g: must be positive", parm.spacing);
	}
	else {
		parm.spacing = 0;
	}

	/*
	 * mask option, solar zenith angle
	 */
	if (got_opt(opt_u)) {
		errno = 0;
		parm.zenith = acos(real_arg(opt_u, 0));
		if (errno)
			error("bad return from acos()");
		if (got_opt(opt_z))
			warn("both -u and -z specified, -z over-ridden");
	}
	else if (got_opt(opt_z)) {
		parm.zenith = zenf(real_arg(opt_z, 0));
	}
	else {
		parm.zenith = 0;
	}

	/*
	 * access input file
	 */
	switch (n_args(efile)) {
	case 0:			/* no operand, open std input	 */
		parm.i_fd = ustdin();
		break;
	case 1:			/* open named file	 	 */
		parm.i_fd = uropen(str_opnd(efile, 0));
		if (parm.i_fd == ERROR)
			error("can't open \"%s\"", str_opnd(efile, 0));
		break;
	default:			/* too many operands	 	 */
		usage();

	}

	/*
	 * can't read or write tty
	 */
	no_tty(parm.i_fd);
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);

	/*
	 * # output bits, just 1 if mask
	 */
	if (got_opt(opt_n)) {
		parm.nbits = int_arg(opt_n, 0);
	} else {
		parm.nbits = (parm.zenith != 0) ? 1 : CHAR_BIT;
	}

	/*
	 * backward flag; default is forward
	 */
	parm.backward = got_opt(opt_b);

	/*
	 * process azimuth direction
	 */
	//	if (parm.backward) {
	//		if (parm.azimuth >= 0)
	//			parm.azimuth -= M_PI;
	//		else
	//			parm.azimuth += M_PI;
	//	}

	/*
	 * read/write headers
	 */
	headers();

	/*
	 * Read the input files, create  output buffer
	 */
	nsamps = hnsamps(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	N =  nsamps * nlines;
	samp_nbytes = sampsize(parm.i_fd);

	zbuf = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
	if (zbuf == NULL) {
		error ("can't allocate input buffer");
	}
	if (fpvread (parm.i_fd, zbuf, N) != N) {
		error ("input image read error");
	}

	//	create output buffer
	fpixel_t *hval;//[nlines];
	hval = (fpixel_t *) ecalloc(N, sizeof(fpixel_t));
	if (hval == NULL) {
		error ("can't allocate output buffer");
	}

	/*
	 * compute horizon
	 */
	horizon(parm, zbuf, hval, samp_nbytes, nlines, nsamps);

	/*
	 * Write to output file
	 */
	if (fpvwrite(parm.o_fd, hval, N) != N) {
		error("error writing output file");
	}

	/*
	 * all done
	 */
//	(void) fpclose(parm.i_fd);
//	(void) fpclose(parm.o_fd);
	ipwexit(EXIT_SUCCESS);
}
