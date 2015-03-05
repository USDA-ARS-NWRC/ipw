#include "ipw.h"
#include "fpio.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "image",
		STR_OPERANDS, "image (default: stdin)",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_v = {
		'v', "values file",
		STR_OPERANDS, "values_file (default: stdin)",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_v,
		0
	};

	int             fdi;	/* input image file descriptor	 */
	int		nread;		/* # input values read		 */
	fpixel_t      **fmap;		/* -> to all F.P. maps		 */
	fpixel_t       *map;		/* band 0 F.P. map		 */
	int	      *fmaplen;		/* -> lengths of all F.P. maps	 */
	int		maplen;		/* length of band 0 F.P. map	 */
	BIH_T         **i_bih;		/* input BIH			 */
	FILE	       *ifp;		/* values file pointer		 */
	pixel_t		value;		/* value to convert		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* open input image */

	if (got_opt (opt_i)) {

		fdi = uropen (str_arg (opt_i, 0));
		if (fdi == ERROR) {
			error ("can't open %s", str_arg (opt_i, 0));
		}
	} else {
		if (!got_opt (opt_v)) {
			error ("At least one of -i,-v must be specified");
		}
		fdi = ustdin();
	}

   /* open values file */

	if (got_opt (opt_v)) {
		ifp = fopen (str_arg (opt_v, 0), "r");
		if (ifp == NULL) {
			error ("can't open %s", str_arg (opt_i, 0));
		}
	} else {
		ifp = stdin;
	}

   /* read input BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read BIH of %s", str_arg (opt_i, 0));
	}

   /* must be single-band image */

	if (hnbands(fdi) != 1) {
		error ("image file %s has more than one band",
			str_arg (opt_i, 0));
	}

   /* ingest LQH */

	gethdrs (fdi, request, NO_COPY, ERROR);
	
   /* check that we read LQH */

	if (hdr_addr(h_lqh) != NULL) {

   		/* get floating point map and length */

		fmap = fpmap (fdi);
		map = fmap[0];
		fmaplen = fpmaplen (fdi);
		maplen = fmaplen[0];
	} else {
		error ("image file %s has no LQH", str_arg (opt_i, 0));
	}

   /* loop reading and converting input values */

	while ((nread = fscanf (ifp, "%d", &value)) != EOF) {
		if (nread != 1) {
			error ("illegal input value");
		}

		printf ("%d\t%f\n", value, map[value]);
	}

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
