/*
** NAME
**	headers -- process image headers
**
*/

#include "ipw.h"
#include "pgm.h"

static FPIXEL lq_mins[1];	/* minimum FPIXEL values for LQ headers */
static FPIXEL lq_maxs[1];	/* maximum FPIXEL values for LQ headers */
static char * lq_units[1];	/* unit strings for LQ headers */

void
headers(
	float	min,	/* minimum value for LQ header */
	float	max,	/* maximum   "    "  "    "    */
	char   *units)	/* units string   "  "    "    */
{
	if (min >= max) {
		error("minimum >= maximum in LQ header");
	}

	lq_mins[0] = min;
	lq_maxs[0] = max;
	lq_units[0] = units;

	/*
	 *  Set the LQ headers of the output image.
	 */
	set_lqh(o_image, lq_mins, lq_maxs, lq_units);
}
