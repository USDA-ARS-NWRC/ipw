/*
 ** NAME
 **	fpvwrite -- write floating-point vectors
 **
 ** SYNOPSIS
 **	#include "fpio.h"
 **
 **	int
 **	fpvwrite(
 **		int             fd,	  |* input file descriptor	 *|
 **		fpixel_t       *buf,	  |* -> pixel buffer		 *|
 **		int             npixv)	  |* # pixel vectors to write	 *|
 **
 ** DESCRIPTION
 **	fpvwrite writes npixv floating-point vectors from buf into file
 **	descriptor fd.
 **
 ** RETURN VALUE
 **	number of floating-point vectors written; else ERROR for failure
 **
 ** GLOBALS ACCESSED
 **	_fpiocb
 */

#include "ipw.h"
#include "_fpio.h"
#include "fpio.h"
#include "pixio.h"

#define	CLAMP(xp, xmin, xmax)	if (*(xp) < (xmin))		\
		*(xp) = (xmin);		\
		else if (*(xp) > (xmax))	\
		*(xp) = (xmax)

#if sun
/*
 * HACK: Sun calls a FUNCTION to do float->unsigned conversion, so we cheat
 * and map through a long.  Suggestions for a cleaner way around this will
 * be gratefully accepted.
 */
typedef long    pixp_t;

#else
typedef pixel_t pixp_t;

#endif

/* ------------------------------------------------------------------------ */

int
fpvwrite(
		int             fd,		/* input file descriptor	 */
		fpixel_t       *buf,		/* -> pixel buffer		 */
		int             npixv)		/* # pixel vectors to write	 */
{
	REG_6 int       band;		/* current band #		 */
	REG_3 int       nbands;		/* # image bands		 */
	REG_5 FPIO_T   *p;		/* -> pixel I/O control block	 */

	p = _fpiocbinit(fd, FPIO_WRITE, npixv);
	if (p == NULL) {
		return (ERROR);
	}

	nbands = p->nbands;
	/*
	 * loop through bands (we expect npixv > nbands)
	 */
#if sun
	/* CONSTCOND */
	assert(sizeof(pixel_t) == sizeof(long));
#endif
	for (band = 0; band < nbands; ++band) {
		REG_1 fpixel_t *bufp;	/* -> current fpixel		 */
		FREG_1 fpixel_t k0;	/* fast floating-pt. coefficient */
		FREG_2 fpixel_t k1;	/* fast floating-pt. coefficient */
		FREG_3 double	kd0;
		FREG_4 double	kd1;
		REG_4 int       npixels;/* # pixels / band		 */
		REG_2 pixp_t   *pixp;	/* -> current pixel		 */

		/*
		 * clamp fpixels to min,max values
		 */
		bufp = &buf[band];
		npixels = npixv;

		k0 = p->fmin[band];
		k1 = p->fmax[band];

		if (nbands == 1) {
			do {
				CLAMP(bufp, k0, k1);
				++bufp;
			} while (--npixels > 0);
		}
		else {
			do {
				CLAMP(bufp, k0, k1);
				bufp += nbands;
			} while (--npixels > 0);
		}

		/*
		 * convert fpixels -> pixels
		 */
		bufp = &buf[band];
		/* NOSTRICT */
		pixp = (pixp_t *) & p->pixbuf[band];
		npixels = npixv;

		/*
		 * if this band is unmapped then just copy the pixels directly
		 */
		if (p->bflags[band] & FPIO_NOMAP) {
			if (nbands == 1) {
				do {
					*pixp++ = *bufp++;
				} while (--npixels > 0);
			}
			else {
				do {
					*pixp = *bufp;

					bufp += nbands;
					pixp += nbands;
				} while (--npixels > 0);
			}
		}

		/*
		 * if there are linear inverse coefficients then use them
		 */
		else if (p->lininv[band] != NULL) {
			kd0 = p->lininv[band][0];
			kd1 = p->lininv[band][1];

			/*
			 * This assertion should be removed after testing.
			 *
			 * (*bufp - kd0) * kd1 will always be positive.  This
			 * means that the float -> unsigned conversion will
			 * always do what we expect.  Also, rounding never
			 * needs to check for positive vs. negative.
			 */
			if (kd0 > (k0+FLT_EPSILON))
				assert(kd1 < 0);

			/*
			 * We now do rounding here
			 */
			if (nbands == 1) {
				do {
					*pixp++ = ((*bufp++ - kd0) * kd1) + 0.5;
				} while (--npixels > 0);
			}
			else {
				do {
					*pixp = ((*bufp - kd0) * kd1) + 0.5;

					bufp += nbands;
					pixp += nbands;
				} while (--npixels > 0);
			}
		}

		/*
		 * otherwise, do a binary search to find the correct pixel
		 *
		 *  Added 8/17/93, Dana Jacobsen, ERL-C.
		 */
		else {
			int left;
			int right;
			int x;

			do {
				left = 0;
				right = p->maplen[band] - 1;
				x = 0;
				while (right >= left) {
					x = (left + right) / 2;
					if (*bufp < p->map[band][x])
						right = x - 1;
					else
						left = x + 1;
				}
				/*  *pixp is between x-1 and x+1  */
				/*  First, check boundaries, then inbetween */
				if (x <= 0) {
					if (x < 0)
						x = 0;
					if (*bufp > ( p->map[band][x] +
							(p->map[band][x+1] - p->map[band][x]) / 2 ))
						*pixp = (pixp_t) x+1;
					else
						*pixp = (pixp_t) x;
				}
				else if (x >= (p->maplen[band] - 1)) {
					if ( x >= p->maplen[band])
						x = (p->maplen[band] - 1);
					if (*bufp > ( p->map[band][x-1] +
							(p->map[band][x] - p->map[band][x-1]) / 2 ))
						*pixp = (pixp_t) x;
					else
						*pixp = (pixp_t) x-1;
				}
				else if ( (*bufp > ( p->map[band][x] +
						(p->map[band][x+1] - p->map[band][x]) / 2 )))
					*pixp = (pixp_t) x+1;
				else if (*bufp > ( p->map[band][x-1] +
						(p->map[band][x] - p->map[band][x-1]) / 2 ))
					*pixp = (pixp_t) x;
				else
					*pixp = (pixp_t) x-1;

				/*
				if (ABS(*bufp - p->map[band][*pixp]) > 0.5) {
				  warn("binary search warning: %d (%d) %f -> %f\n", *pixp, x, *bufp, p->map[band][*pixp]);
				}
				 */

				bufp += nbands;
				pixp += nbands;
			} while (--npixels > 0);
		}
		/* end replacement */
	}

	/*
	 * write pixel vectors
	 */
	if (pvwrite(fd, p->pixbuf, npixv) != npixv) {
		return (ERROR);
	}

	/*
	 * return # pixel vectors written
	 */
	return (npixv);
}
