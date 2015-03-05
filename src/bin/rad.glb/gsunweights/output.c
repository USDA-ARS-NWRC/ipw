/*
** NAME
** 	output -- copy temporary file to output image for sunweights program
** 
** SYNOPSIS
**	output (tempfile, nbands, fdo)
** 	char *tempfile;
**	int nbands;
**	int fdo;
** 
** DESCRIPTION
** 	output copies the temporary file to the output image.
** 
*/

#include "ipw.h"
#include "fpio.h"

void
output (
	char	       *tempfile,	/* name of temporary output file */
	int		nbands,		/* # bands in output image	 */
	int		fdo)		/* output image file descriptor  */
{
	fpixel_t       *buf;		/* -> I/O buffer		 */
	int             fdt;		/* temporary file descriptor	 */
	int             npix;		/* # pixels to write		 */
	int             nbytes;		/* # bytes to read		 */


  /* open temporary file */

	if ((fdt = uropen (tempfile)) == ERROR) {
		error ("can't re-open temporary file");
	}

   /* allocate I/O buffer - largest buffer possible divisible by nbands */

	npix = BUFSIZ;
	npix = (npix / nbands) * nbands;

	buf = (fpixel_t *) ecalloc (npix, sizeof(fpixel_t));
	if (buf == NULL) {
		error ("can't create I/O buffer");
	}
	nbytes = npix * sizeof(fpixel_t);

   /* plow through temporary file */

	while ((npix = uread (fdt, (addr_t) buf, nbytes)) > 0) {
		npix /= (sizeof(fpixel_t) * nbands);
		if (fpvwrite (fdo, buf, npix) != npix) {
			error ("write error - output file likely incomplete");
		}
	}
	if (npix < 0) {
		error ("read error on temporary file - output file likely incomplete");
	}

   /* clean up */

	(void) uclose (fdt);
}
