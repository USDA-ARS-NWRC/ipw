/*
** NAME
**      ipw2pgm -- reads IPW file and converts to PGM
**
*/
#include "ipw.h"
#include "pixio.h"
#include "pgm.h"

void
ipw2pgm(void)
{
  pixel_t     * mb;
  pixel_t     * mbp;		/* pointer to mask image	*/
  pixel_t     * ib;
  pixel_t     * ibp;		/* pointer to input image 	*/
  char        * ob;
  char        * obp;		/* pointer to output image	*/
  long          line, samp;
  float         scale;
  char          osbuf[100];


        /* allocate mask buffers */

  if (parm.imask != ERROR) {
    mb = (pixel_t *) ecalloc (parm.nsamps, sizeof(pixel_t));
    if (mb == NULL) {
      error ("Unable to allocate mask buffer");
    }
  } else
    mb = NULL;

        /* allocate image input and output buffers */

  ib = (pixel_t *) ecalloc (parm.nsamps, sizeof(pixel_t));
  if (ib == NULL) {
    error ("Unable to allocate input buffer for image");
  }

  ob = (char *) ecalloc (parm.nsamps, sizeof(char));
  if (ob == NULL) {
    error ("Unable to allocate input buffer for image");
  }

	/* write binary PGM header */

  (void) sprintf(osbuf, "%s\n%d %d\n%d\n",
                 "P5", parm.nsamps, parm.nlines, 255);
  uputs(parm.output, osbuf);

  scale = 256.0 / ipow2(parm.nbits);

  for (line = 0; line < parm.nlines; line++) {
    mbp = mb;
    ibp = ib;
    obp = ob;
    if (parm.imask != ERROR)
      if (pvread (parm.imask, mb, parm.nsamps) != parm.nsamps)
        error ("Error reading mask image, line %d", line);
    if (pvread (parm.im, ib, parm.nsamps) != parm.nsamps )
      error ("Error reading image data, line %d", line);
    for (samp = 0; samp < parm.nsamps; samp++) {
      if (parm.imask == ERROR || *mbp) {
        *obp++ = (char) (*ibp * scale);
      } else {
        *obp++ = (char) 0;
      }
      mbp++;
      ibp++;
    }
    (void) uwrite(parm.output, (addr_t) ob, (int) (obp - ob) );
  }

  (void) free(ib);
  if (parm.imask != ERROR) {
    (void) free (mb);
  }
  (void) free (ob);

}
