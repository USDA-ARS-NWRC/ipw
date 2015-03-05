/*
** NAME
**      ipw3ppm -- converts 3 band IPW files to PPM
**
** SEE ALSO
**	ipwload
**
*/
#include "ipw.h"
#include "pixio.h"
#include "pgm.h"

#define CBARSIZE 20

void
ipw3ppm(void)
{
  char        * ob;
  char        * obp;
  pixel_t     * ibp;		/* pointer to image  		*/
  pixel_t     * mbp;		/* pointer to mask image	*/
  long          line, samp;
  float         scale0;
  float         scale1;
  float         scale2;
  char          osbuf[100];


        /* allocate mask buffers */

  if (parm.imask != ERROR) {
    parm.maskd = (pixel_t *) ecalloc (parm.nsamps, sizeof(pixel_t));
    if (parm.maskd == NULL) {
      error ("Unable to allocate mask buffer");
    }
  } else
    parm.maskd = NULL;

        /* allocate image input buffers */

  parm.imd = (pixel_t *) ecalloc (parm.nsamps * 3, sizeof(pixel_t));
  if (parm.imd == NULL) {
    error ("Unable to allocate input buffer for image");
  }

  ob = (char *) ecalloc (parm.nsamps * 3, sizeof (char));
  if (ob == NULL) {
    error ("Unable to allocate output buffer for image");
  }

  (void) sprintf(osbuf, "%s\n%d %d\n%d\n",
                 "P6", parm.nsamps, parm.nlines, 255);
  uputs(parm.output, osbuf);

  scale0 = 256.0 / ipow2(parm.nbits0);
  scale1 = 256.0 / ipow2(parm.nbits1);
  scale2 = 256.0 / ipow2(parm.nbits2);

  for (line = 0; line < parm.nlines; line++) {
    ibp = parm.imd;
    obp = ob;
    if (pvread (parm.im1, ibp, parm.nsamps) != parm.nsamps )
      error ("Error reading image data, line %d", line);
    if (parm.imask == ERROR) {
      for (samp = 0; samp < parm.nsamps; samp++) {
        *obp++ = (char) (*(ibp+0) * scale0);
        *obp++ = (char) (*(ibp+1) * scale1);
        *obp++ = (char) (*(ibp+2) * scale2);
        ibp += 3;
      }
    } else {
      mbp = parm.maskd;
      if (pvread (parm.imask, mbp, parm.nsamps) != parm.nsamps)
        error ("Error reading mask image, line %d", line);
      for (samp = 0; samp < parm.nsamps; samp++) {
        if (*mbp++) {
          *obp++ = (char) (*(ibp+0) * scale0);
          *obp++ = (char) (*(ibp+1) * scale1);
          *obp++ = (char) (*(ibp+2) * scale2);
        } else {
          *obp++ = (char) 0;
          *obp++ = (char) 0;
          *obp++ = (char) 0;
        }
        ibp += 3;
      }
    }
    (void) uwrite(parm.output, (addr_t) ob, (int) (obp - ob) );
  }

  (void) uclose(parm.im1);

  if (parm.imask != ERROR)
    (void) uclose(parm.imask);

  if (parm.verbose)
    fprintf(stderr, "%d x %d x %d bit image.\n",
                    parm.nlines, parm.nsamps, parm.nbits);

  (void) free (ob);
  (void) free (parm.imd);
  if (parm.imask != ERROR)
    (void) free (parm.maskd);
}
