#include <string.h>

#include "ipw.h"
#include "image.h"
#include "_image.h"
#include "lqh.h"
#include "fpio.h"

IMAGE_T *
open_image(
	char       *name,	/* name of image file */
	int	    access)	/* access level */
{
	IMAGE_T    *icb;	/* -> new image's control block		*/
	int	    b;		/* loop counter for bands		*/

	assert((access == PIXEL_ACCESS) || (access == FPIXEL_ACCESS));

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

	/*
	 *  Are we opening standard input?
	 */
	if (STREQ(name, IPW_STDIN)) {
		icb->fd = ustdin();
	}
	else {
		icb->fd = uropen(name);
		if (icb->fd == ERROR) {
			_free_img_cb(icb);
			ipw_errno = IPWERR_MESSAGE;
			return NULL;
		}
	}

	icb->flags |= (access | IPW_READ);

	/*
	 *  Read in the image's headers
	 */
	if (! _read_hdrs(icb)) {
		close(icb->fd);
		_free_img_cb(icb);
		return NULL;
		}

	/*
	 *  Fetch some basic info from the BI headers.
	 */
	icb->nlines   = bih_nlines(icb->bih[0]);
	icb->nsamples = bih_nsamps(icb->bih[0]);
	icb->nbands   = bih_nbands(icb->bih[0]);
	icb->nbits    = (int *) ecalloc(icb->nbands, sizeof(int));
	if (icb->nbits == NULL) {
		close(icb->fd);
		_free_img_cb(icb);
		ipw_errno = IPWERR_MEMORY;
		return NULL;
	}
	for (b = 0; b < icb->nbands; b++) {
		icb->nbits[b] = bih_nbits(icb->bih[b]);
	}


	/*
	 *  If accessing floating-point pixels, do we have LQ headers?
	 */
	if (access == FPIXEL_ACCESS) {
	    if (icb->lqh == NULL) {
		warn("input image '%s' has no LQ headers; raw values used",
		     image_name(icb));
	    } else {
		/*
		 *  Check each LQ header to make sure it has only two
		 *  breakpoints; essentially a minimum and a maximum.
		 */
		for (b = 0; b < icb->nbands; b++) {
		    if (icb->lqh[b] == NULL) {
			warn("input image '%s', band %d has no LQ header;"
			     " raw values used", image_name(icb), b);
		    }
		    if (lqh_nbkpts(icb->lqh[b]) > 2) {
			error("input image '%s', band %d has more than 2"
			      " breakpoints", image_name(icb), b);
		    }
		}

		/*
		 *  Set pointers to min and max arrays.
		 */
		icb->lqh_mins = fpfmin(icb->fd);
		icb->lqh_maxs = fpfmax(icb->fd);
		if ((icb->lqh_mins == NULL) || (icb->lqh_maxs == NULL)) {
		    close(icb->fd);
		    _free_img_cb(icb);
		    ipw_errno = IPWERR_MESSAGE;
		    return NULL;
		}
	    }
	} else {
	    icb->lqh_mins = NULL;
	    icb->lqh_maxs = NULL;
	}

	return icb;
}
