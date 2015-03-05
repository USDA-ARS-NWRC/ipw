#include <string.h>

#include "ipw.h"
#include "image.h"
#include "_image.h"

IMAGE_T *
new_image(
	char       *name,	/* name of image file			*/
	int	    nlines,	/* # of lines in image			*/
	int	    nsamples,	/* # of samples per line		*/
	int	    nbands,	/* # of bands per sample		*/
	int	   *nbits)	/* # of bits for each band (array)	*/
{
	IMAGE_T    *icb;	/* -> new image's control block		*/
	int	    b;		/* loop counter for bands		*/

	assert(name != NULL);
	assert(nlines > 0);
	assert(nsamples > 0);
	assert(nbands > 0);
	assert(nbits != NULL);

	for (b = 0; b < nbands; b++) {
		if (nbits[b] <= 0) {
			error("nbits[%d] <= 0", b);
		}
		else if (nbits[b] > (sizeof(PIXEL) * CHAR_BIT)) {
			error("nbits[%d] > size of a pixel", b); 
		}
	}

	icb = _new_img_cb();
	if (icb == NULL) {
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}

	icb->name = strdup(name);
	if (icb->name == NULL) {
		_free_img_cb(icb);
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}

	if (STREQ(name, IPW_STDOUT)) {
		icb->fd = ustdout();
	}
	else {
		icb->fd = uwopen(name);
		if (icb->fd == ERROR) {
			_free_img_cb(icb);
			ipw_errno = IPWERR_MESSAGE;
			return NULL;
		}
	}

	icb->nlines = nlines;
	icb->nsamples = nsamples;
	icb->nbands = nbands;
	icb->flags |= (PIXEL_ACCESS | IPW_WRITE);

	icb->nbits = (int *) ecalloc(nbands, sizeof(int));
	if (icb->nbits == NULL) {
		uclose(icb->fd);
		_free_img_cb(icb);
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}
	for (b = 0; b < nbands; b++) {
		icb->nbits[b] = nbits[b];
	}

	/*
	 *  Allocate & initialize BI headers.
	 */
	icb->bih = (BIH_T **) ecalloc(nbands, sizeof(BIH_T *));
	if (icb->bih == NULL) {
		uclose(icb->fd);
		_free_img_cb(icb);
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}

	/*
	 *  Make BI header for band 0, then use it to create BI
	 *  headers for others (this insures that their "per-image"
	 *  component is the same -- pain in the butt if you ask me).
	 */
	icb->bih[0] = bihmake(0, nbits[0], NULL, NULL, NULL,
			      nlines, nsamples, nbands);
	if (icb->bih[0] == NULL) {
		uclose(icb->fd);
		_free_img_cb(icb);
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}
	for (b = 1; b < nbands; b++) {
		icb->bih[b] = bihmake(0, nbits[b], NULL, NULL,
				      icb->bih[0],
				      nlines, nsamples, nbands);
		if (icb->bih[b] == NULL) {
			uclose(icb->fd);
			_free_img_cb(icb);
			ipw_errno = IPWERR_MEMORY;
			return NULL;
		}
	}

	return icb;
}
