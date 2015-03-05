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
	static OPTION_T operand = {
		OPERAND, "image",
		STR_OPERANDS, "image",
		REQUIRED, 1
	};

	static OPTION_T *optv[] = {
		&operand,
		0
	};

	char	       *filename;	/* name of current image file	 */
	int             fdi;		/* input image file descriptor	 */
	int		ifile;		/* index of current file	 */
	int		first_lqh;	/* flag for first LQH found	 */
	fpixel_t	min =0.0;	/* minimum F.P. map value	 */
	fpixel_t	max =0.0;	/* maximum F.P. map value	 */
	fpixel_t       *fpmin;		/* min value F.P. map		 */
	fpixel_t       *fpmax;		/* max value F.P. map		 */
	BIH_T         **i_bih;		/* input BIH			 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);


	first_lqh = TRUE;

	for (ifile = 0; ifile < n_args(operand); ifile++) {

		filename = str_arg (operand, ifile);

		/* open image */

		fdi = uropen (filename);
		if (fdi == ERROR) {
			error ("can't open %s", filename);
		}

   		/* read BIH */

		if ((i_bih = bihread(fdi)) == NULL) {
			error ("can't read BIH of %s", filename);
		}

   		/* must be single-band image */

		if (hnbands(fdi) != 1) {
			error ("image file %s has more than one band", filename);
		}

   		/* ingest LQH */

		gethdrs (fdi, request, NO_COPY, ERROR);
	
   		/* check that we read LQHs */

		if (hdr_addr(h_lqh) != NULL) {

   			/* get min and max floating point maps */

			fpmin = fpfmin (fdi);
			fpmax = fpfmax (fdi);

			if (!first_lqh) {
				min = MIN (min, fpmin[0]);
				max = MAX (max, fpmax[0]);
			} else {
				first_lqh = FALSE;
				min = fpmin[0];
				max = fpmax[0];
			}
		} else {
			warn ("image file %s has no LQH", filename);
		}

	}

   /* print min and max */

	printf ("%f %f\n", min, max);

	ipwexit (EXIT_SUCCESS);
}
