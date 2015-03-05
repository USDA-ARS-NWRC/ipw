/*
** NAME
**	headers -- process image headers
**
*/

#include "ipw.h"
#include "pgm.h"

void
headers(void)
{
	/*
	 *  Copy the histories from input image to output image.
	 */
	set_history(o_image, 0, dupsv(history(i_image, 0)));

	/*
	 *  Copy any GEO headers from input image to output image.
	 */
	set_geoh(o_image, 0, get_geoh(i_image, 0));
}
