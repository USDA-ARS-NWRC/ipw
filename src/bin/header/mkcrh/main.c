#include "ipw.h"
#include "crh.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_c = {
		'c', "file containing lo,hi,rep triples",
		STR_OPTARGS, "class_file",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_a = {
		'a', "class annotation",
		STR_OPTARGS, "annot",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
		'u', "data units",
		STR_OPTARGS, "units",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "floor value",
		INT_OPTARGS, "floor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "ceiling value",
		INT_OPTARGS, "ceil",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "band #s to receive classes (default: all)",
		INT_OPTARGS, "band",
		OPTIONAL, 1
	};

	static OPTION_T opt_f = {
		'f', "force header output; ignore any input image",
	};

	static OPTION_T opt_i = {
		'i', "input image file",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_c,
		&opt_l,
		&opt_h,
		&opt_a,
		&opt_u,
		&opt_b,
		&opt_f,
		&opt_i,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int             nclass;		/* # classes         		 */
	int		floor;		/* class floor value		 */
	int		ceil;		/* class ceiling value		 */
	CLASS		class[MAX_CLASS]; /* class data for each class	 */
	char           *annot;		/* annotation                 	 */
	char	       *units;		/* units			 */
	FILE	       *cfp;		/* pointer to class file         */


   /* begin */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

   /* access input file(s) */

	if (!got_opt(opt_i)) {
		if (!got_opt(opt_c)) {
			fprintf (stderr, "ERROR: at least one of (-c,-i) must be specified");
			usage();
		}
		fdi = ustdin();
	} else {
		fdi = uropen(str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}
	no_tty(fdi);

	if (got_opt(opt_c)) {
		cfp = fopen (str_arg(opt_c, 0), "r");
		if (cfp == NULL) {
			error("can't open \"%s\"", str_arg(opt_c, 0));
		}
	} else {
		cfp = stdin;
	}

	annot = got_opt(opt_a) ? str_arg(opt_a, 0) : (char *) NULL;
	units = got_opt(opt_u) ? str_arg(opt_u, 0) : (char *) NULL;

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* get lo, hi, rep value triples */

	get_classes (cfp, &nclass, class);

	if (got_opt(opt_l))
		floor = int_arg(opt_l, 0);
	else
		floor = 0;

	if (got_opt(opt_h))
		ceil = int_arg(opt_h, 0);
	else
		ceil = nclass;


   /* do it */

	mkcrh (fdi, nclass, class, floor, ceil, annot, units,
	       n_args(opt_b), int_argp(opt_b), got_opt(opt_f), fdo);

   /* done */

	ipwexit(EXIT_SUCCESS);
}
