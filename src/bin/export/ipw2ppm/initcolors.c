/*
** NAME
**      initcolors -- initializes standard colors
**
** DESCRIPTION
**
*/
#include "ipw.h"
#include "pgm.h"

void
initcolors(void)
{
  parm.gblack.r = 0;
  parm.gblack.g = 0;
  parm.gblack.b = 0;

  parm.gwhite.r = 255;
  parm.gwhite.g = 255;
  parm.gwhite.b = 255;

  parm.gred.r = 255;
  parm.gred.g = 0;
  parm.gred.b = 0;

  parm.ggreen.r = 0;
  parm.ggreen.g = 255;
  parm.ggreen.b = 0;

  parm.gblue.r = 0;
  parm.gblue.g = 0;
  parm.gblue.b = 255;
}
