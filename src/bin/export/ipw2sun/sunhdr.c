#include "ipw.h"

#include "rasterfile.h"
#include "pgm.h"

int convertToBigEndian(int x)
{
#if WORDS_BIGENDIAN
	return x;
#else
	int result = x;
	char* p = (char *) &result;
	char temp = p[0];
	p[0] = p[3];
	p[3] = temp;
	temp = p[1];
	p[1] = p[2];
	p[2] = temp;
	return result;
#endif
}

/*
 * sunhdr -- write Sun raster image file header
 */

int
sunhdr(
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / line	 	 */
	int             nbits,		/* # bits / pixel		 */
	int             o_fd)		/* output image file descriptor	 */
{
	int             nbits_line;	/* # bits / line		 */
	int             nbytes_line;	/* # bytes / line		 */

	struct rasterfile ras;		/* Sun rasterfile header	 */

 /* NOSTRICT */
	ras.ras_magic = convertToBigEndian(RAS_MAGIC);

	ras.ras_width = convertToBigEndian(nsamps);
	ras.ras_height = convertToBigEndian(nlines);
	ras.ras_depth = convertToBigEndian(nbits);

	nbits_line = nsamps * nbits;
 /*
  * Sun bitmap lines are rounded up to a multiple of NBITS_ROUND bits
  */
	nbits_line += NBITS_ROUND - 1;
	nbits_line /= NBITS_ROUND;
	nbits_line *= NBITS_ROUND;

	nbytes_line = nbits_line / CHAR_BIT;

	ras.ras_length = convertToBigEndian(nlines * nbytes_line);
	ras.ras_type = convertToBigEndian(RT_STANDARD);

	if (nbits == 1) {
		ras.ras_maptype = convertToBigEndian(RMT_NONE);
		ras.ras_maplength = convertToBigEndian(0);
	}
	else {
		ras.ras_maptype = convertToBigEndian(RMT_EQUAL_RGB);
		ras.ras_maplength = convertToBigEndian(3 * 256);
	}
 /*
  * write rasterfile header
  */
 /* NOSTRICT */
	if (uwrite(o_fd, (addr_t) (&ras), sizeof(ras)) != sizeof(ras)) {
		error("can't write rasterfile header");
	}
 /*
  * if 8-bit pixels then write fake (1:1) colormap
  */
	if (nbits == 8) {
		int		i;	/* loop counter			*/

		char		map[256];	/* colormap array	*/

		for (i = 0; i < sizeof(map); ++i) {
			map[i] = i;
		}

		for (i = 0; i < 3; ++i) {
			if (uwrite(o_fd, map, sizeof(map)) != sizeof(map)) {
				error("can't write fake colormap");
			}
		}
	}
 /*
  * return rasterfile line size
  */
	return (nbytes_line);
}
