#include "ipw.h"
#include "image.h"

PIXEL *
new_pbuf(
	int	nsamples,	/* # of samples in buffer	*/
	int	nbands)		/* # of bands per sample	*/
{
	PIXEL  *p;		/* new line buffer		*/

	assert(nsamples > 0);
	assert(nbands > 0);

	p = (PIXEL *) ecalloc(nsamples * nbands, sizeof(PIXEL));
	return p;
}
