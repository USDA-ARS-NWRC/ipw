#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(void)
{
  BIH_T     ** i_bih;           /* -> image BIH                  */
  BIH_T     ** m_bih;           /* -> mask  BIH                  */
 
        /* read BIH */
 
  if ((i_bih = bihread (parm.im1)) == NULL) {
    error ("can't read BIH of image");
  }
 
        /* check BIH */
 
  if (bih_nbands (i_bih[0]) != 1) {
    error ("image has %d bands; single band images only", bih_nbands(i_bih[0]));
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
                
	/* write output BIH */

  if (bihwrite(parm.outfile, i_bih) == ERROR) {
    error("can't write basic image header");
  }

        /* ingest LQH for first image, copy everything to output image */
 
  fphdrs (parm.im1, bih_nbands (i_bih[0]), parm.outfile);

	/* end header of output image */

  if (boimage(parm.outfile) == ERROR) {
    error("can't terminate header output");
  }

}
