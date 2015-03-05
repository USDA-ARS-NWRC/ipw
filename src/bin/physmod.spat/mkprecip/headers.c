/*
** NAME
**	headers -- process image headers
**
*/

#include "ipw.h"
#include "pgm.h"

static char * annotations[] = {
	"m_pp: total precipitation mass (kg/m^2)",
	"percent_snow: % of mass that was snow (0 to 1.0)",
	"rho_snow: density of snow (kg/m^3)",
	"T_pp: temperature of precipitation (C)"
};

/*
** note: if the ranges in calc_snow.c for output snow density
**       are changed, the min/max values must also be changed
*/

static FPIXEL mins[] = {
	0.0,
	0.0,
	75,
	-40
};

static FPIXEL maxs[] = {
	100.0,
	1.0,
	250,
	25
};

static char * units[] = {
	"kg/m^2",
	NULL,
	"kg/m^3",
	"C"
};

void
headers(void)
{
	int	   	b;		/* loop counter for bands	 */
	STRVEC_T       *annot;		/* band annotation		 */

	/*
	 *  band 0 of output image = band 0 of input image
	 *  band 3 of output image = band 1 of input image
	 */

	/*
	 *  Copy the histories from input image to output image.
	 */
	set_history(o_image, 0, dupsv(history(i_image, 0)));
	set_history(o_image, 3, dupsv(history(i_image, 1)));

	/*
	 *  Setup annotations in output image using annotations from input
	 *  image.
	 */
	for (b = 0; b < OBANDS; b++) {
		switch (b) {
		    case 0 :
			annot = dupsv(annotation(i_image, 0));
			break;
		    case 3 :
			annot = dupsv(annotation(i_image, 1));
			break;
		    default :
			annot = NULL;
			break;
		}
		set_annotation(o_image, b, addsv(annot, annotations[b]));
	}

	/*
	 *  Set the LQ headers of the output image.
	 */
	set_lqh(o_image, mins, maxs, units);

	/*
	 *  Copy any GEO headers from input image to output image.
	 */
	set_geoh(o_image, 0, get_geoh(i_image, 0));
	set_geoh(o_image, 3, get_geoh(i_image, 1));
}
