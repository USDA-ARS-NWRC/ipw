/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the Computer Systems Laboratory, University of
 * California, Santa Barbara and its contributors'' in the documentation
 * or other materials provided with the distribution and in all
 * advertising materials mentioning features or use of this software.
 *
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "ipw.h"
#include "bih.h"

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
	int		obands;		/* # output bands		*/

	obands = hnbands(parm.o_fd);
/*
 * open temporary file
 */
	if ((fdt = uropen(parm.tmpnam)) == ERROR) {
		error("can't re-open temporary file");
	}
 /*
  * allocate I/O buffer
  */
	npix = BUFSIZ;
 /* NOSTRICT */
	buf = (fpixel_t *) ecalloc(npix * obands, sizeof(fpixel_t));
	if (buf == NULL) {
		error("can't create I/O buffer");
	}
	nbytes = npix * obands * sizeof(fpixel_t);
 /*
  * plow through temporary file
  */
 /* NOSTRICT */
	while ((npix = uread(fdt, (addr_t) buf, nbytes)) > 0) {
		npix /= obands * sizeof(fpixel_t);
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
	SAFE_FREE(buf);
}
