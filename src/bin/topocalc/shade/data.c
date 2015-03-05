/*
 * variables passed externally between
 *	buffers()
 *	cosines()
 *	headers()
 *	init_shade()
 */

#include "ipw.h"
#include "fpio.h"
#include "pixio.h"

pixel_t       **shade;		/* store already computed values */
pixel_t        *obuf;		/* binary fraction output buffer */
float          *cosdtbl;	/* all values of cos(phi-A)	 */
float          *costbl;		/* all possible values of cosS	 */
fpixel_t       *sintbl;		/* all possible values of sinS	 */
int             nget;		/* # pixels to read		 */
pixel_t        *ibuf;		/* binary frac slope/azm buffers */
