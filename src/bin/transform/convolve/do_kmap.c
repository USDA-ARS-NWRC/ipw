#include "ipw.h"

#include "pgm.h"

/*
 * do_kmap -- initialize kernel (weight*fpixel)[pixel] maps
 */

fpixel_t     ***
do_kmap(
	fpixel_t      **kernel,		/* -> kernel array		 */
	int             nrows,		/* # kernel rows		 */
	int             ncols,		/* # kernel columns		 */
	fpixel_t       *map,		/* -> fpixel[pixel] map		 */
	int             maplen)		/* # map elements		 */
{
	REG_1 fpixel_t ***kmap;		/* -> kernel map array		 */
	int             row;		/* current kernel row counter	 */

	assert(map != NULL);
	assert(maplen > 0);
 /*
  * allocate kernel map pointer array
  */
 /* NOSTRICT */
	kmap = (fpixel_t ***) allocnd(sizeof(fpixel_t *), 2, nrows, ncols);
	if (kmap == NULL) {
		error("can't allocate kernel map pointer array");
	}
 /*
  * loop through kernel rows
  */
	for (row = 0; row < nrows; ++row) {
		REG_6 int       col;	/* current kernel column	 */

		if (kernel[row] == NULL) {
			continue;
		}
 /*
  * loop through kernel columns
  */
		for (col = 0; col < ncols; ++col) {
			REG_2 int       c;	/* column index		 */
			REG_5 int       i;	/* map index		 */
			REG_4 fpixel_t *mp;	/* -> weight*fpixel map	 */
			REG_3 int       r;	/* row index		 */
			FREG_1 fpixel_t val;	/* kernel[row][col]	 */

			val = kernel[row][col];
			if (val == 0) {
				continue;
			}
 /*
  * compare kernel[row][col] against kernel value already processed -- if a
  * match is found, use the previously computed map
  */
			for (r = 0; r < row; ++r) {
				if (kernel[r] == NULL) {
					continue;
				}

				for (c = 0; c < ncols; ++c) {
					if (val == kernel[r][c]) {
						mp = kmap[r][c];
						assert(mp != NULL);

						goto found;
					}
				}
			}

			for (c = 0; c < col; ++c) {
				if (val == kernel[row][c]) {
					mp = kmap[row][c];
					assert(mp != NULL);

					goto found;
				}
			}
 /*
  * no match -- compute a new map
  */
 /* NOSTRICT */
			mp = (fpixel_t *) ecalloc(maplen, sizeof(fpixel_t));
			if (mp == NULL) {
				error("can't allocate kernel map array");
			}

			for (i = 0; i < maplen; ++i) {
				mp[i] = map[i] * val;
			}
	found:
			kmap[row][col] = mp;
		}
	}

	return (kmap);
}
