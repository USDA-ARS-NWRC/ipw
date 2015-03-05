/*
** NAME
**      geohfree -- free all left over parts of a GEO header
**
** SYNOPSIS
**      #include "geoh.h"
**
**      int
**	geohfree(
**      	GEOH_T **geohpp,
**		int      nbands)
**
** DESCRIPTION
**	geohfree frees an entire geohpp structure.
**
** RETURN VALUE
**      OK for success, ERROR for failure
*/

#include "ipw.h"
#include "geoh.h"

int
geohfree(
	GEOH_T ** geohpp,
	int       nbands)
{
  int band;

  if (geohpp == NULL)
    return (ERROR);

  for (band = 0; band < nbands; band++) {
    GEOH_T  *geohp;

    geohp = geohpp[band];
    if (geohp == NULL)
      continue;

    SAFE_FREE(geohp->units);
    SAFE_FREE(geohp->csys);
    SAFE_FREE(geohp);
  }

  SAFE_FREE(geohpp);

  return (OK);
}
