/*
**  Sample "headers.c" file
*/

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

/*
**  NAME
**	headers - process image headers
**
**  SYNOPSIS
**	#include "pgm.h"
**
**	void
**	headers(void)
**
**  DESCRIPTION
**	{This function processes all the image headers.  It reads in the
**	 headers of the input (and mask) images, and using those headers
**	 constructing the headers of the output image.}
**
**  RETURN VALUE
**	None.
**
**  GLOBALS READ
**	I_fd
**	O_fd
*/

void
headers(void)
{
	BIH_T  ** i_bihpp;	/* -> input BIH array		 */
	BIH_T  ** o_bihpp;	/* -> output BIH array		 */

	/*
	 * read BIH
	 */
	i_bihpp = bihread(I_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	/*
	 * create and write BIH
	 */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}

	if (bihwrite(O_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}

/*
** (process remaining headers)
*/

	/*
	 * done
	 */
	if (boimage(O_fd) == ERROR) {
		error("can't terminate header output");
	}
}
