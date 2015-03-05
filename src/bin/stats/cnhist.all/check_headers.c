/*
** NAME
** 	check_headers -- read/check input headers for cnhist.all
** 
** SYNOPSIS
**	#include "getargs.h"
**
**	check_headers (nfiles, operands, fdm, nbits, nlines, nsamps)
** 	int nfiles;
**	OPTION_T operands;
**	int fdm;
**	int *nbits, *nlines, *nsamps;
** 
** DESCRIPTION
** 	check_headers reads and checks the headers of the IPW input image(s)
**	for the cnhist.all program.
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "fpio.h"
#include "bih.h"
#include "pgm.h"

void
check_headers (
	int		nfiles,		/* # input images 		 */
	OPTION_T	operands,	/* input image operand struct	 */
	int		fdm,		/* mask file descriptor		 */
	int	       *nbits,		/* returned # bits in images	 */
	int	       *nlines,		/* returned # lines in images	 */
	int	       *nsamps)		/* returned # samples in images	 */
{
	int		fdi;		/* file desc for 1st input image */
	int		fdt;		/* input image file desc	 */
	int		i;		/* file loop counter		 */
	fpixel_t	idum;		/* dummy variable		 */
	BIH_T         **i_bih;		/* -> input BIH			 */
	BIH_T         **m_bih;		/* -> mask BIH			 */
	fpixel_t       *fmins;		/* -> mins of each F.P. map	 */
	fpixel_t       *fmaxs;		/* -> maxs of each F.P. map	 */
	fpixel_t	min;		/* min in F.P. map of 1st image	 */
	fpixel_t	max;		/* max in F.P. map of 1st image	 */


   /* open first input image */

	if (nfiles > 0) {
		fdi = uropen (str_arg (operands, 0));
		if (fdi == ERROR) {
			error ("Unable to open file %s", str_arg (operands, 0));
		}
	} else {
		fdi = ustdin();
	}
	no_tty (fdi);

   /* read BIH of first input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read BIH of file %s", str_arg (operands, 0));
	}

   /* check BIH of first image */

	if (bih_nbands (i_bih[0]) != 1) {
		error ("file %s has more than one band; single band images only",
			str_arg (operands, 0));
	}

	*nlines = bih_nlines (i_bih[0]);
	*nsamps = bih_nsamps (i_bih[0]);
	*nbits = bih_nbits (i_bih[0]);

   /* read/check BIH of mask image, if specified */

	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (bih_nbands (m_bih[0]) > 1) {
			error ("mask image has > 1 bands");
		}
		if ((bih_nlines (m_bih[0]) != *nlines) ||
		    (bih_nsamps (m_bih[0]) != *nsamps)) {
			error ("mask image has incompatible # dimensions");
		}
		skiphdrs (fdm);
	}
		
   /* ingest LQH for first image, other headers are skipped */

	fphdrs (fdi, NO_COPY, ERROR);

   /* Get min and max of floating point map of 1st input image */

	fmins = fpfmin (fdi);
	fmaxs = fpfmax (fdi);
	min = fmins[0];
	max = fmaxs[0];

   /* close first input image */
   /* do a dummy read so pxclose won't complain */

	fpvread (fdi, &idum, 1);
	fpclose (fdi);

   /* If only one image, nothing more to check */

	if (nfiles <= 1) {
		return;
	}

   /* Read headers of remaining images, comparing to first */

	for (i = 1; i < nfiles; i++) {

		/* open next image */

		fdt = uropen (str_arg (operands, i));
		if (fdt == ERROR) {
			error ("Unable to open %s", str_arg (operands, i));
		}

   		/* read BIH of input image */

		if ((i_bih = bihread (fdt)) == NULL) {
			error ("can't read BIH of %s", str_arg (operands, i));
		}

   		/* check BIH of first image */

		if (bih_nbands (i_bih[0]) != 1) {
			error ("file %s has more than one band; single band images only",
				str_arg (operands, i));
		}

		/* Check #bits and dimensions */

		if ((bih_nlines (i_bih[0]) != *nlines) ||
		    (bih_nsamps (i_bih[0]) != *nsamps) ||
		    (bih_nbits (i_bih[0]) != *nbits)) {
			error ("input images have incompatible #bits or dimensions");
		}

   		/* ingest LQH for first image, other headers are skipped */

		fphdrs (fdt, NO_COPY, ERROR);
	
		/* Check min and max of floating point map */

		fmins = fpfmin (fdt);
		fmaxs = fpfmax (fdt);
		if (fmins[0] != min || fmaxs[0] != max) {
			error ("input images have incompatible floating point quantization");
		}

		/* close image */
   		/* do a dummy read so pxclose won't complain */

		fpvread (fdt, &idum, 1);
		fpclose (fdt);
	}
}
