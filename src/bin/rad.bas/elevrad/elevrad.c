#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 ** NAME
 **	elevrad -- look up and write out values of beam and diffuse
 **
 ** SYNOPSIS
 **	#include "file.h"
 **
 **	elevrad(fdi, fdo)
 **	int fdi, fdo;
 **
 ** DESCRIPTION
 **	Elevrad reads RAW pixel_t elevation values, applies the look-up
 **	tables beam and diffuse, and writes the fpixel_t output image.
 **
 ** GLOBALS ACCESSED
 **	beam and diffuse look-up tables
 **
 ** FUTURE DIRECTIONS
 **	Possibly write a look-up table instead and pipe output through
 **	lutx.
 **
 */

void
elevrad(
		int             fdi,		/* input file descriptor	 */
		int             fdo)		/* output file descriptor	 */
{
	REG_1 pixel_t  *ip;		/* -> ibuf			 */
	REG_2 fpixel_t *op;		/* -> obuf			 */
	REG_3 int       j;		/* counter			 */
	REG_4 fpixel_t *pb;		/* -> beam			 */
	REG_5 fpixel_t *pd;		/* -> diffuse			 */
	fpixel_t       *obuf;		/* output buffer		 */
	int             nget;		/* # pixels to read		 */
	int             nwrite;		/* # pixels to write		 */
	pixel_t        *ibuf;		/* input buffer			 */

	/*
	 * allocate buffers
	 */
	assert(hnbands(fdi) == 1);
	assert(hnbands(fdo) == 2);
	nget = hnsamps(fdi);
	/* NOSTRICT */
	ibuf = (pixel_t *) ecalloc(nget, sizeof(pixel_t));
	assert(ibuf != NULL);
	/* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(nget * 2, sizeof(pixel_t));
	assert(obuf != NULL);
	/*
	 * read/write till done
	 */
	pb = beam;
	pd = diffuse;
	while ((nwrite = pvread(fdi, ibuf, nget)) > 0) {
		ip = ibuf;
		op = obuf;
		for (j = nwrite; --j >= 0;) {
			/*
			 * first band is beam, second diffuse
			 */
			*op++ = pb[*ip];
			*op++ = pd[*ip++];
		}
		if (fpvwrite(fdo, obuf, nwrite) != nwrite) {
			error("write error");
		}
	}

	SAFE_FREE(ibuf);
	SAFE_FREE(obuf);
}
