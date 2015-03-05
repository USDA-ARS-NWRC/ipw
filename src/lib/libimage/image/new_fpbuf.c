#include "ipw.h"
#include "image.h"

FPIXEL *
new_fpbuf(
	int	     	nsamples,	/* # of samples in buffer	*/
	int		nbands)		/* # of bands per sample	*/
{
	FPIXEL         *p;		/* new buffer			*/

	assert(nsamples > 0);
	assert(nbands > 0);

	p = (FPIXEL *) ecalloc(nsamples * nbands, sizeof(FPIXEL));
	return p;
}
