/*
** NAME
**      orhfree -- free all left over parts of a OR header
**
** SYNOPSIS
**      #include "orh.h"
**
**      int
**	orhfree(
** 		ORH_T **orhpp,
**		int     nbands)
**
** DESCRIPTION
**	orhfree frees an entire orhpp structure.
**
** RETURN VALUE
**      OK for success, ERROR for failure
*/

#include "ipw.h"
#include "orh.h"

int
orhfree(
	ORH_T **  orhpp,
	int       nbands)
{
  int band;

  if (orhpp == NULL)
    return (ERROR);

  for (band = 0; band < nbands; band++) {
    ORH_T  *orhp;

    orhp = orhpp[band];
    if (orhp == NULL)
      continue;

    SAFE_FREE(orhp->orient);
    SAFE_FREE(orhp->origin);
    SAFE_FREE(orhp);
  }

  SAFE_FREE(orhpp);

  return (OK);
}
