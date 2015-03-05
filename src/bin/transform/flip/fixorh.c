/*
** fix orientation header
*/

#include <string.h>
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "pgm.h"

 /* codes for diagonals */
static int      diag[] = {0, 3, 4, 1, 2};

 /* codes for samples in normal orientation */
static int      xcol[] = {0, 2, 1, 4, 3};

 /* codes for lines in normal orientation */
static int      xrow[] = {0, 4, 3, 2, 1};

static ORH_T   *
neworh(void)
{
	ORH_T          *orhp;		/* -> orientation header	 */

	if (parm.lines && parm.samps) {
		orhp = orhmake(IPW_ORIENT, ORIG_3);
	}
	else if (parm.lines) {
		orhp = orhmake(IPW_ORIENT, ORIG_4);
	}
	else if (parm.samps) {
		orhp = orhmake(IPW_ORIENT, ORIG_2);
	}
 /*
  * shouldn't reach, error will be caught in main
  */
	else {
		bug("(shouldn't reach: neither -l nor -s specified");
	}
	return (orhp);
}

ORH_T         **
fixorh(i_orh)
	ORH_T         **i_orh;		/* -> input ORH array			 */
{
	ORH_T          *orhp0;		/* -> orientation header	 */
	ORH_T         **o_orh;		/* -> output ORH array		 */
	bool_t          needed;		/* ? ORH needed			 */
	int             band;		/* band index			 */
	int             nbands;		/* # bands in input file	 */
	int             icode;		/* code for input origin	 */
	int             ocode;		/* code for output origin	 */

	nbands = hnbands(parm.i_fd);
	icode = 0;
	ocode = 0;
 /*
  * no existing orientation header, so create one
  */

	if (i_orh == (ORH_T **) NULL) {
 /* NOSTRICT */
		o_orh = (ORH_T **) hdralloc(nbands, sizeof(ORH_T *),
					    parm.o_fd, ORH_HNAME);
		orhp0 = neworh();
 /*
  * copy to all bands
  */
		for (band = 0; band < nbands; ++band) {
			o_orh[band] = orhp0;
		}
	}

	else {
 /*
  * duplicate existing header, then fix
  */
		o_orh = orhdup(i_orh, nbands);
 /*
  * change existing orientation header for each band
  */
		for (band = 0; band < nbands; ++band) {
 /*
  * if this band doesn't have an orientation header, create one
  */
			if (o_orh[band] == (ORH_T *) NULL) {
				o_orh[band] = neworh();
			}
 /*
  * numerical code for input origin 1,2,3,4 clockwise from upper left
  */
			else if (streq(orh_origin(o_orh[band]), ORIG_1)) {
				icode = 1;
			}
			else if (streq(orh_origin(o_orh[band]), ORIG_2)) {
				icode = 2;
			}
			else if (streq(orh_origin(o_orh[band]), ORIG_3)) {
				icode = 3;
			}
			else if (streq(orh_origin(o_orh[band]), ORIG_4)) {
				icode = 4;
			}
 /*
  * numerical code for output origin
  */

			if (parm.lines && parm.samps) {
				ocode = diag[icode];
			}

			else if (parm.lines) {
				ocode = streq(orh_orient(o_orh[band]), ROW) ?
					xrow[icode] : xcol[icode];
			}

			else if (parm.samps) {
				ocode = streq(orh_orient(o_orh[band]), ROW) ?
					xcol[icode] : xrow[icode];
			}
 /*
  * insert string descriptor for output origin
  */
			SAFE_FREE( orh_origin(o_orh[band]) );

			switch (ocode) {
			case 1:
				orh_origin(o_orh[band]) = strdup(ORIG_1);
				break;
			case 2:
				orh_origin(o_orh[band]) = strdup(ORIG_2);
				break;
			case 3:
				orh_origin(o_orh[band]) = strdup(ORIG_3);
				break;
			case 4:
				orh_origin(o_orh[band]) = strdup(ORIG_4);
				break;
			default:
				bug("shouldn't reach: invalid ocode");
			}
			if (orh_origin(o_orh[band]) == NULL) {
				error("can't duplicate origin field");
			}
		}
	}
 /*
  * Check output header.  If we've created an image in standard IPW
  * format, set the header to NULL
  */
	needed = FALSE;
	for (band = 0; band < nbands; ++band) {
		if (strdiff(IPW_ORIENT, orh_orient(o_orh[band])) ||
		    strdiff(IPW_ORIGIN, orh_origin(o_orh[band]))) {
			needed = TRUE;
		}
		else {
			o_orh[band] = NULL;
		}
	}

	return (needed == FALSE ? (ORH_T **) NULL : o_orh);
}
