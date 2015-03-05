#include "ipw.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 * output -- read temporary file and write output file
 */

void
output(void)
{
	fpixel_t       *buf;		/* -> I/O buffer		 */
	int             fdt;		/* temporary file descriptor	 */
	int             npix;		/* # pixels to write		 */
	int             nbytes;		/* # bytes to read		 */

/*
 * open temporary file
 */

	if ((fdt = uropen(parm.tmpnam)) == ERROR) {
		error("can't re-open temporary file");
	}
 /*
  * allocate I/O buffer
  */
	npix = BUFSIZ * parm.obands;
 /* NOSTRICT */
	buf = (fpixel_t *) ecalloc(npix, sizeof(fpixel_t));
	if (buf == NULL) {
		error("can't create I/O buffer");
	}
	nbytes = npix * sizeof(fpixel_t);
 /*
  * plow through temporary file
  */
 /* NOSTRICT */
	while ((npix = uread(fdt, (addr_t) buf, nbytes)) > 0) {
		npix /= (sizeof(fpixel_t) * parm.obands);
		if (fpvwrite(parm.o_fd, buf, npix) != npix) {
			error("write error - output file likely incomplete");
		}
	}
	if (npix < 0) {
		error("read error on temporary file - output file likely incomplete");
	}
 /*
  * clean up
  */
	(void) uclose(fdt);
	(void) uremove(parm.tmpnam);
}
