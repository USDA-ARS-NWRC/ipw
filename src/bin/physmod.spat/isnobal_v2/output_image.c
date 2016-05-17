/*
 ** NAME
 ** 	output_image -- create output image file
 **
 ** SYNOPSIS
 ** 	#include "pgm.h"
 **
 **	int
 **	output_image (
 **	    char	*filename,	|* name of new output image	   *|
 **	    int		 nbands,	|* # bands in output image	   *|
 **	    char       **units,		|* units string for each band	   *|
 **	    char       **annots,	|* annotation string for each band *|
 **	    fpixel_t    *mins,		|* min value for each band	   *|
 **	    fpixel_t    *maxs)		|* max value for each band	   *|
 **
 ** DESCRIPTION
 ** 	output_image creates an output image with the given filename.
 **	This routine outputs the headers for the new image.  The array
 **	of annotation strings is used for the basic image header.  The
 **	arrays of units, minimums and maximums are used for LQ headers.
 **
 ** RETURN VALUE
 **	>= 0	The file descriptor of the new output image.
 **
 ** GLOBAL VARIABLES READ
 ** 	ic_bih
 ** 	ic_geoh
 **
 ** GLOBAL VARIABLES MODIFIED
 */

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "geoh.h"
#include "fpio.h"
#include "pgm.h"

int
output_image (
		char	       *filename,	/* name of new output image	   */
		int		nbands,		/* # bands in output image	   */
		char          **units,		/* units string for each band	   */
		char          **annots,		/* annotation string for each band */
		fpixel_t       *mins,		/* min value for each band	   */
		fpixel_t       *maxs,		/* max value for each band	   */
		int 			nbits)							/* number of bits */
{
	int		fd;		/* output image file desc	   */
	BIH_T         **o_bih;		/* output image BIH		   */
	GEOH_T        **o_geoh;		/* output image GEOH		   */
	int             band;		/* band loop counter		   */


	/* open output image file */

	fd = uwopen(filename);
	if (fd == ERROR) {
		error("Can't open output image file '%s'", filename);
	}

	/* create output BIH (copy of input BIH), # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc(nbands, sizeof(BIH_T *), fd, BIH_HNAME);
	for (band = 0; band < nbands; band++) {
		o_bih[band] = bihmake(0, nbits, bih_history(ic_bih[0]),
				addsv(NULL, annots[band]),
				o_bih[0],
				bih_nlines(ic_bih[0]),
				bih_nsamps(ic_bih[0]),
				nbands);
	}

	/* write output BIH */

	if (bihwrite(fd, o_bih) == ERROR) {
		error("Can't write BIH for image '%s'", filename);
	}

	/* duplicate input GEO header and write to output image */

	if (ic_geoh != NULL) {
		o_geoh = (GEOH_T **) hdralloc(nbands, sizeof(GEOH_T *), fd,
				GEOH_HNAME);
		for (band = 0; band < nbands; band++) {
			o_geoh[band] = ic_geoh[0];
		}
		if (geohwrite(fd, o_geoh) == ERROR) {
			error("Can't write GEOH for image '%s'", filename);
		}
	}

	/* create LQH for output image */

	newlqh(fd, nbands, mins, maxs, units);

	return fd;
}
