#include "ipw.h"
#include "image.h"
#include "_image.h"
#include "hdrio.h"
#include "fpio.h"

void
set_lqh(
	IMAGE_T        *image,		/* output image			 */
	FPIXEL          mins[],		/* array of minimums for bands	 */
	FPIXEL          maxs[],		/* array of maximums for bands	 */
	char          **units)		/* array of units for bands	 */
{
	int             b;		/* loop counter for bands	 */
	pixel_t         ibkpt[2];	/* integer break points		 */
	fpixel_t        fbkpt[2];	/* FP break points		 */

	assert(image != NULL);
	assert(image->flags & IPW_WRITE);

	for (b = 0; b < image->nbands; b++) {
		if (mins[b] >= maxs[b]) {
			error("band %d: minimum (%f) >= maximum (%f)", b,
				mins[b], maxs[b]);
		}
	}
 
	/*
	 *  Has this routine already been called for this image?
	 */
	if (image->flags & FPIXEL_ACCESS) {
		bug("already set LQ headers for output image");
	}

	image->flags |= FPIXEL_ACCESS;
	image->lqh_mins = mins;
	image->lqh_maxs = maxs;

	/*
	 *  Create LQ headers.
	 */
	assert(image->lqh == NULL);
	image->lqh = (LQH_T **) ecalloc(image->nbands, sizeof(LQH_T *));
	if (image->lqh == NULL) {
		error(ipw_strerr(IPWERR_MEMORY));
	}
	for (b = 0; b < image->nbands; b++) {
		ibkpt[0] = 0;
		ibkpt[1] = ipow2(image->nbits[b]) - 1;
		fbkpt[0] = mins[b];
		fbkpt[1] = maxs[b];

		image->lqh[b] = lqhmake(image->nbits[b], 2, ibkpt, fbkpt,
					(units != NULL) ? units[b] : NULL,
				   	NULL);
		if (image->lqh[b] == NULL) {
			error(ipw_strerr(IPWERR_MEMORY));
		}
	}
}
