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

void
headers(void)
{
  BIH_T     ** i_bih;		/* -> image BIH			 */
  BIH_T     ** m_bih;		/* -> mask  BIH			 */
  char         fromfile[256];

        /* read BIH of first input image */

  if ((i_bih = bihread (parm.im1)) == NULL) {
    error ("can't read BIH of image 1");
  }

        /* check BIH of first image */

  if (bih_nbands (i_bih[0]) != 1) {
    error ("image 1 has more than one band; single band images only");
  }

  parm.nlines = bih_nlines (i_bih[0]);
  parm.nsamps = bih_nsamps (i_bih[0]);
 
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
		
        /* ingest LQH for first image, other headers are skipped */

  fphdrs (parm.im1, NO_COPY, ERROR);

        /* read BIH of next image */

  if ((i_bih = bihread (parm.im2)) == NULL) {
    error ("can't read BIH of image 2");
  }

        /* check BIH of second image */

  if (bih_nbands (i_bih[0]) != 1) {
    error ("image 2 has more than one band; single band images only");
  }

        /* Check dimensions */

  if ((bih_nlines (i_bih[0]) != parm.nlines) ||
      (bih_nsamps (i_bih[0]) != parm.nsamps)) {
    error ("input images has incompatible dimensions");
  }

  if (bih_annot (i_bih[0]) == NULL) {
    (void) sprintf (fromfile, "file %s", ufilename(parm.im2) );
    parm.annot = addsv (NULL, fromfile);
  } else {
    parm.annot = dupsv (bih_annot (i_bih[0]));
  }

  fphdrs (parm.im2, NO_COPY, ERROR);

  parm.maxn = parm.nlines * parm.nsamps;
}
