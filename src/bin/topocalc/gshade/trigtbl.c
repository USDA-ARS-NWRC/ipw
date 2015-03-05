/*
** NAME
** 	trigtbl -- initialize trigonmetric lookup tables for gshade
** 
** SYNOPSIS
**	void trigtbl (fdi, fds, sintbl, costbl, zsintbl, zcostbl, zenith)
** 	int fdi, fds;
**	fpixel_t *sintbl;
**	float *costbl, *zsintbl, *zcostbl;
**	bool_t zenith;
** 
** DESCRIPTION
** 	trigtbl initializes lookup tables for sines and cosines of all
**	possible slopes and solar zenith angles in the input images to
**	program gshade.
** 
*/

#include <math.h>
#include "ipw.h"
#include "fpio.h"

void
trigtbl (
	int             fdi,		/* input file descriptor	 */
	int             fds,		/* solar file descriptor	 */
	fpixel_t       *sintbl,		/* all possible values of sinS	 */
	float          *costbl,		/* all possible values of cosS	 */
	float          *zsintbl,	/* all possible values of sinZ	 */
	float          *zcostbl,	/* all possible values of cosZ	 */
	bool_t		zenith)		/* flag set if zenith in image   */
{
	FREG_1 double   x;		/* sinS or cosZ			 */
	REG_1 int	j;		/* loop counter and B.F.	 */
	fpixel_t      **map;		/* -> F.P. maps			 */
	fpixel_t       *zmap;		/* -> F.P. map of zenith band	 */
	int            *maplen;		/* -> lengths of F.P. maps	 */
	int		max;		/* length of F.P. map (max B.F.) */
	float	       *pcos;		/* -> in cos array		 */
	float	       *psin;		/* -> in sin array		 */
	double		zr;		/* zenith in radians		 */
	double		pi;

	pi = M_PI;

   /* sines and cosines of each possible slope */

	map = fpmap (fdi);
	maplen = fpmaplen (fdi);
	max = maplen[0];

	pcos = costbl;
	for (j = 0; j < max; j++) {
		x = sintbl[j];
		*pcos++ = sqrt ((1 - x) * (1 + x));
	}

   /* sines and cosines of each possible zenith angle */
   /* if zenith is set, band 1 is zenith in degrees; else band 1 is cos zenith */

	map = fpmap (fds);
	zmap = map[0];
	maplen = fpmaplen (fds);
	max = maplen[0];

	pcos = zcostbl;
	psin = zsintbl;

	if (!zenith) {
		for (j = 0; j < max; j++) {
			x = *pcos++;
			*psin++ = sqrt ((1 - x) * (1 + x));
		}
	} else {
		for (j = 0; j < max; j++) {
			zr = *zmap * pi / 180;
			*pcos++ = cos (zr);
			*psin++ = sin (zr);
			zmap++;
		}
	}
}
