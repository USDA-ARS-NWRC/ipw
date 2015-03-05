/*
 * create new LQH
 */

#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"

LQH_T         **
newlqh(
	int             fdo,		/* output file desc		 */
	bool_t          slope,		/* ? compute slope 		 */
	bool_t          aspect)		/* ? compute aspect 		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	fpixel_t        fbkpt[2][2];	/* limits of LQ			 */
	pixel_t         ibkpt[2][2];	/* integer break points		 */
	char          **units;		/* units			 */
	double          pi;
	int             j;

 /*
  * preamble
  */
 /* NOSTRICT */
	units = (char **) ecalloc(2, sizeof(char *));
	if (units == NULL) {
		return (NULL);
	}
	pi = M_PI;
 /*
  * if slope to be computed
  */
	if (slope) {
		fbkpt[0][0] = 0;
		fbkpt[0][1] = 1;
		units[0] = NULL;
		if (aspect) {
			fbkpt[1][1] = pi *
				(1 - ldexp(1.0, 1 - hnbits(fdo, 1)));
			fbkpt[1][0] = -pi;
			units[1] = "radians";
		}
	}
 /*
  * must be aspects only
  */
	else if (aspect) {
		fbkpt[0][1] = pi *
			(1 - ldexp(1.0, 1 - hnbits(fdo, 0)));
		fbkpt[0][0] = -pi;
		units[0] = "radians";
	}
 /*
  * programmer brain damage
  */
	else {
		bug("neither slope nor aspect");
	}
 /*
  * return the new LQH
  */
	ibkpt[0][0] = ibkpt[1][0] = 0;
	ibkpt[0][1] = ipow2(hnbits(fdo, 0)) - 1;
	if (hnbands(fdo) > 1)
		ibkpt[1][1] = ipow2(hnbits(fdo, 1)) - 1;
 /* NOSTRICT */
	lqhpp = (LQH_T **) hdralloc(hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	for (j = hnbands(fdo); --j >= 0;)
		lqhpp[j] = lqhmake(hnbits(fdo, j), 2,
				ibkpt[j], fbkpt[j], units[j], (char *) NULL);

	SAFE_FREE(units);

	return (lqhpp);
}
