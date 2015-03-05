/*
** NAME
** 	read_headers -- read input headers for cnhist.all
** 
** SYNOPSIS
**	read_headers (fdi)
** 	int fdi;
** 
** DESCRIPTION
** 	read_headers reads the BIH and LQH and skips the remaining headers
**	of the given IPW image
** 
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pgm.h"

void
read_headers (
	int		fdi)		/* input image file descriptor	 */
{

   /* read BIH of input image */

	if (bihread (fdi) == NULL) {
		error ("can't read input BIH");
	}

   /* ingest LQH and skip remaining headers */

	fphdrs (fdi, NO_COPY, ERROR);
}
