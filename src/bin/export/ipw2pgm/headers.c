/*
** NAME
**      headers -- read headers
**
** DESCRIPTION
**      Reads BIH and LQH of input images.  Reads mask headers.  Checks for
**      consistency.
**
*/
#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

void
headers(void)
{
  BIH_T     ** i_bih;		/* -> image BIH			 */
  BIH_T     ** m_bih;		/* -> mask  BIH			 */

        /* read BIH */

  if ((i_bih = bihread (parm.im)) == NULL) {
    error ("can't read BIH of image.");
  }

  parm.nlines = bih_nlines (i_bih[0]);
  parm.nsamps = bih_nsamps (i_bih[0]);
  parm.nbands = bih_nbands (i_bih[0]);
  parm.nbits  = bih_nbits  (i_bih[0]);
  parm.maxn  = parm.nlines * parm.nsamps;

  if (parm.nbands != 1)
    error ("image must have 1 band.");

  skiphdrs (parm.im);

        /* read/check BIH of mask image, if specified */

  if (parm.imask != ERROR) {
    if ((m_bih = bihread (parm.imask)) == NULL) {
      error ("can't read BIH of mask image");
    }
    if (bih_nbands (m_bih[0]) > 1) {
      error ("mask image has > 1 bands");
    }
    if ((bih_nlines (m_bih[0]) != parm.nlines) ||
        (bih_nsamps (m_bih[0]) != parm.nsamps)) {
      error ("mask image has incompatible dimensions");
    }
    skiphdrs (parm.imask);
  }

    if (parm.verbose)
      fprintf(stderr, "%d x %d x %d bit image\n",
                      parm.nlines, parm.nsamps, parm.nbits);

}
