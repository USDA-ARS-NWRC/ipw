/*
** NAME
** 	copy_image -- copy temporary file to image file
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	copy_image(
**	    char       *tempfile,	|* name of temporary output file *|
**	    int		nbands,		|* # bands in output image	 *|
**	    fpixel_t   *buf,		|* buffer to hold fpixel data	 *|
**	    int		fdo)		|* output image file descriptor  *|
** 
** DESCRIPTION
** 	copy_image copies the temporary file to the output image.
** 
** GLOBAL VARIABLES READ
**	nsamps
**
** GLOBAL VARIABLES MODIFIED
**
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
copy_image(
	char	       *tempfile,	/* name of temporary output file */
	int		nbands,		/* # bands in output image	 */
	fpixel_t        *buf,		/* buffer to hold fpixel data	 */
	int		fdo)		/* output image file descriptor  */
{
	int             fdt;		/* file desc for temporary file	 */
	int             nbytes;		/* # bytes to read		 */
	int             n;		/* status returned by uread	 */


  /* open temporary file */

	if ((fdt = uropen(tempfile)) == ERROR) {
		error("can't re-open temporary file");
	}

   /* determine the # of bytes in the buffer */

	nbytes = nsamps * nbands * sizeof(fpixel_t);

   /* plow through temporary file */

	while ((n = uread(fdt, (addr_t) buf, nbytes)) > 0) {
		if (fpvwrite(fdo, buf, nsamps) != nsamps) {
			error("write error - output file incomplete");
		}
	}
	if (n < 0) {
		error("read error on temporary file - output file incomplete");
	}

   /* clean up */

	(void) uclose(fdt);
	(void) fpclose(fdo);
}
