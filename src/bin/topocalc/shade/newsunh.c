/*
 * create new SUNH
 */

#include "ipw.h"
#include "hdrio.h"
#include "sunh.h"

SUNH_T        **
newsunh(
	int             fdo,		/* output file descriptor	 */
	double          czen,		/* cosine solar zenith		 */
	double          azm)		/* solar azimuth, radians	 */
{
	SUNH_T        **sunh;		/* -> output SUNH		 */
	
 /* NOSTRICT */
	sunh = (SUNH_T **) hdralloc(1, sizeof(SUNH_T *), fdo, SUNH_HNAME);
	if (sunh == NULL)
		return (NULL);

	sunh[0] = sunhmake(czen, azm);
	if (sunh[0] == NULL)
		return (NULL);

	return (sunh);
}
