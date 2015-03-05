/*
** NAME
**      rgblookup -- looks up a color for a given pixel value
**
** DESCRIPTION
**
*/
#include <math.h>

#include "ipw.h"
#include "pgm.h"

colors
rgblookup(
	int	pix)
{
  int       mapno = 0;
  int       i;
  double    pixval;
  double    percent;
  colors    c;

  pixval = parm.fpm[pix];
  for (i = 0; (i < parm.nD) && (!mapno); i++)
    if ( (pixval >= parm.D[i].value) && (pixval <= parm.D[i+1].value) )
      mapno = i+1;
  if (!mapno) return (parm.gwhite);
  percent =   (pixval              - parm.D[mapno-1].value)
            / (parm.D[mapno].value - parm.D[mapno-1].value);
  c.r =  rint( percent * (parm.D[mapno].c.r - parm.D[mapno-1].c.r)
                + parm.D[mapno-1].c.r );
  c.g =  rint( percent * (parm.D[mapno].c.g - parm.D[mapno-1].c.g)
                + parm.D[mapno-1].c.g );
  c.b =  rint( percent * (parm.D[mapno].c.b - parm.D[mapno-1].c.b)
                + parm.D[mapno-1].c.b );
  return (c);
}
