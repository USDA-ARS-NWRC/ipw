/*
** NAME
**      qipw2pgm -- outputs PGM header and copies image data
**
*/
#include "ipw.h"
#include "pixio.h"

#include "pgm.h"

void
qipw2pgm(void)
{
  char          osbuf[100];


	/* write binary PGM header */

  (void) sprintf(osbuf, "%s\n%d %d\n%d\n",
                 "P5", parm.nsamps, parm.nlines, 255);
  uputs(parm.output, osbuf);

  if (ucopy(parm.im, parm.output, parm.maxn) == ERROR)
    error ("Copy of image data failed.");
 
}
