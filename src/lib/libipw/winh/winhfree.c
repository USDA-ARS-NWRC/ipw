/*
** NAME
**      winhfree -- free all left over parts of a WIN header
**
** SYNOPSIS
**      #include "winh.h"
**
**	int
**	winhfree(
**		WINH_T **  winhpp,
**		int        nbands)
**
** DESCRIPTION
**	winhfree frees an entire winhpp structure.
**
** RETURN VALUE
**      OK for success, ERROR for failure
*/

#include "ipw.h"
#include "winh.h"

int
winhfree(
     	WINH_T **  winhpp,
	int        nbands)
{
  int band;

  if (winhpp == NULL)
    return (ERROR);

  for (band = 0; band < nbands; band++) {
    WINH_T  *winhp;

    winhp = winhpp[band];
    if (winhp == NULL)
      continue;

    SAFE_FREE(winhp);
  }

  SAFE_FREE(winhpp);

  return (OK);
}
