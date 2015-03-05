#include "ipw.h"
#include "image.h"

GEOH_T *
set_geoh(
	IMAGE_T	       *image,		/* ou put image	*/
	int		band,		/* band #	*/
	GEOH_T         *geoh)		/* GEO header   */
{
	int		b;		/* loop index   */

	assert(image != NULL);
	assert((0 <= band) && (band < image->nbands));

	/*
	 *  If no GEO headers yet, allocate the array for pointers.
	 */
	if (image->geoh == NULL) {
		image->geoh = (GEOH_T **) ecalloc(image->nbands,
						  sizeof(GEOH_T *));
		if (image->geoh == NULL) {
			error(ipw_strerr(IPWERR_MEMORY));
		}
		for (b = 0; b < image->nbands; b++) {
			image->geoh[b] = NULL;
		}
	}

	/*
	 *  Is there already a GEO header for this band?
	 */
	assert(image->geoh[band] == NULL);

	image->geoh[band] = geoh;
}
