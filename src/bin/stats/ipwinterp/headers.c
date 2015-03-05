#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "pgm.h"

void
headers()
{
  BIH_T     ** i_bih;		/* -> image BIH			 */
  BIH_T     ** m_bih;		/* -> mask  BIH			 */

        /* read BIH */

  if ((i_bih = bihread (parm.im1)) == NULL) {
    error ("can't read BIH of image");
  }

        /* check BIH */

  if (bih_nbands (i_bih[0]) != 1) {
    error ("image has more than one band; single band images only");
  }

  parm.nlines = bih_nlines (i_bih[0]);
  parm.nsamps = bih_nsamps (i_bih[0]);
 
        /* read/check BIH of mask image, if specified */

  if (parm.maskfile != ERROR) {
    if ((m_bih = bihread (parm.maskfile)) == NULL) {
      error ("can't read BIH of mask image");
    }
    if (bih_nbands (m_bih[0]) > 1) {
      error ("mask image has > 1 bands");
    }
    if ((bih_nlines (m_bih[0]) != parm.nlines) ||
        (bih_nsamps (m_bih[0]) != parm.nsamps)) {
      error ("mask image has incompatible dimensions");
    }
    skiphdrs (parm.maskfile);
  }
		
        /* ingest LQH for first image, other headers are skipped */

  fphdrs (parm.im1, NO_COPY, ERROR);

  parm.maxn = parm.nlines * parm.nsamps;
}
