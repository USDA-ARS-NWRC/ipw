/*
 ** NAME
 ** 	open_input -- open an input file
 **
 ** SYNOPSIS
 **	#include "pgm.h"
 **
 **	int
 **	open_input(
 **	    char       *prefix,		|* prefix for file name		 *|
 **	    int		index,		|* 1st index for file extension	 *|
 **	    bool_t     *sun_up)		|* flag for net solar rad data	 *|
 **
 ** DESCRIPTION
 **	open_input opens an input images named "prefix.index".	The file
 **	is initialized for floating point image input (the headers are
 **	read and checked).  The sun_up flag is set to TRUE if the opened
 **	image contains net solar radiation data; FALSE otherwise.
 ** 	If an error occurs, the routine prints an error message and
 **	exits the application.
 **
 ** RETURN VALUE
 ** 	>= 0	File descriptor of opened input image
 **
 ** GLOBAL VARIABLES READ
 **
 ** GLOBAL VARIABLES MODIFIED
 */

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"
#include "pgm.h"

UNITS_T update_units[] = {
		U_WATTS_PER_SQUARE_METER,
		U_CELSIUS,
		U_PASCALS,
		U_METERS_PER_SECOND,
		U_CELSIUS,
		U_WATTS_PER_SQUARE_METER
};

int
open_input(
		char	       *prefix,		/* prefix for file name		 */
		int		index,		/* index for file extension	 */
		bool_t	       *sun_up)		/* flag for net solar rad data	 */
{
	int	        fd;		/* returned file descriptor	 */
	char		filename[255];	/* buffer for filename		 */
	BIH_T         **i_bih;		/* -> input BIH			*/

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


	/* form file name */

	sprintf(filename, "%s.%0*d", prefix, nDigits, index);

	/* open file */

	fd = uropen (filename);
	if (fd == ERROR) {
		error("unable to open %s", filename);
	}

	/* read/check headers */

	/* read BIH of image */

	if ((i_bih = bihread (fd)) == NULL) {
		error("can't read BIH of file %s", filename);
	}

	/* check input BIH - set sun_up flag */

	if (hnbands(fd) == IBANDS) {
		*sun_up = TRUE;

	} else if (hnbands(fd) == (IBANDS-1)) {
		*sun_up = FALSE;

	} else {
		error("image %s not valid for this program: wrong number bands",
				filename);
	}

	if ((bih_nlines(i_bih[0]) != nlines) ||
			(bih_nsamps(i_bih[0]) != nsamps))
		error("input file %s has incompatible dimensions", filename);

	/* ingest LQH, other headers are ignored */
	/* call gethdrs instead of fphdrs, so we can access the LQ headers (JBD,
      Nov 95) */

	gethdrs(fd, request, NO_COPY, ERROR);

	/* check the units of the LQ headers */
	if (units_warn)
		check_units((LQH_T **) hdr_addr(h_lqh), update_units,
				hnbands(fd), fd);

	return fd;
}
