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
#include "fpio.h"
#include "pgm.h"

#define MAXBITS 21

void
headers(void)
{
  BIH_T     ** i_bih;		/* -> image BIH			 */
  BIH_T     ** m_bih;		/* -> mask  BIH			 */

        /* read BIH */

  if ((i_bih = bihread (parm.im1)) == NULL) {
    error ("can't read BIH of image.");
  }

  parm.nlines = bih_nlines (i_bih[0]);
  parm.nsamps = bih_nsamps (i_bih[0]);
  parm.nbands = bih_nbands (i_bih[0]);
  parm.nbits  = bih_nbits  (i_bih[0]);
  parm.maxn  = parm.nlines * parm.nsamps;

  if ( (parm.nbands != 1) && (parm.nbands != 3) )
    error ("image must have 1 or 3 bands.");

  if (parm.nbits > MAXBITS)
    error ("image has over %d bits per pixel.  Please reduce it.", MAXBITS);

  parm.fpm = NULL;
  if (parm.nbands == 1) {
    fphdrs (parm.im1, NO_COPY, ERROR);
    parm.fpm = fpmap(parm.im1)[0];
  } else {
    parm.nbits0 = bih_nbits (i_bih[0]);
    parm.nbits1 = bih_nbits (i_bih[1]);
    parm.nbits2 = bih_nbits (i_bih[2]);
    skiphdrs(parm.im1);
  }

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
}
