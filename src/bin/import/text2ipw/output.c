/*
** NAME
** 	output -- copy temporary file to output image for iimport program
** 
** SYNOPSIS
**	output (tempfile, fdo, nbands)
** 	char *tempfile;
**	int fdo;
**	int nbands;
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
	int		fdo,		/* output image file descriptor  */
	int		nbands)		/* # bands in output image	 */
{
	fpixel_t       *buf;		/* -> I/O buffer		 */
	int             fdt;		/* temporary file descriptor	 */
	int             npix;		/* # pixels to write		 */
	int             nbytes;		/* # bytes to read		 */
	int		nread;		/* # bytes actually read	 */
	int		bytes_per_pix;	/* # bytes per image pixel	 */


  /* re-open temporary file */

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

	bytes_per_pix = sizeof(fpixel_t) * nbands;

	while ((nread = uread (fdt, (addr_t) buf, nbytes)) > 0) {
		npix = nread / bytes_per_pix;
		if (fpvwrite (fdo, buf, npix) != npix) {
			error ("write error - output file likely incomplete");
		}
	}
	if (npix < 0) {
		error ("read error on temporary file - output file likely incomplete");
	}

   /* clean up */

	(void) uclose (fdt);
	(void) uremove (tempfile);
	SAFE_FREE(buf);
}
