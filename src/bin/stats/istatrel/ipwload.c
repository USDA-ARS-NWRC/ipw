/*
** NAME
**      ipwload -- opens files and reads values into arrays
**
** DESCRIPTION
**      reads mask
**      allocates image input buffers (parm.samps wide)
**      reads images
**      frees up buffers and closes image files
**
*/
#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
ipwload(
	double	*ax,
	double	*ay)
{
  fpixel_t     * ibuf1;		/* image 1 input vector		*/
  fpixel_t     * ibuf2;		/* image 2 input vector		*/
  fpixel_t     * ib1_p;		/* pointer to image 1 		*/
  fpixel_t     * ib2_p;		/* pointer to image 2 		*/
  pixel_t      * mbuf;		/* entire mask image		*/
  pixel_t      * mbuf_p;	/* pointer to mask image	*/
  long           line, samp;
  long           nval;


        /* read mask */

  if (parm.imask != ERROR) {
    mbuf = (pixel_t *) ecalloc (parm.maxn, sizeof(pixel_t));
    if (mbuf == NULL) {
      error ("Unable to allocate mask buffer");
    }
    if (pvread (parm.imask, mbuf, parm.maxn) != parm.maxn) {
      error ("Error reading mask image");
    }
  } else {
    mbuf = NULL;
  }

        /* allocate image input buffers */

  ibuf1 = (fpixel_t *) ecalloc (parm.nsamps, sizeof (fpixel_t));
  if (ibuf1 == NULL) {
    error ("Unable to allocate input buffer for image 1");
  }

  ibuf2 = (fpixel_t *) ecalloc (parm.nsamps, sizeof (fpixel_t));
  if (ibuf2 == NULL) {
    error ("Unable to allocate input buffer for image 2");
  }

        /* read images */

  if (parm.verbose)
    fprintf(stderr, "reading images...max %ld points\n", parm.maxn);

  nval = 0;
  mbuf_p = mbuf;

  for (line = 0; line < parm.nlines; line++ ) {
    if ( fpvread (parm.im1, ibuf1, parm.nsamps) != parm.nsamps )
      error ("Error reading input image 1, line %d", line);

    if ( fpvread (parm.im2, ibuf2, parm.nsamps) != parm.nsamps )
      error ("Error reading input image 2, line %d", line);

    ib1_p = ibuf1;
    ib2_p = ibuf2;

    for (samp = 0; samp < parm.nsamps; samp++) {
      if (parm.imask == ERROR || *mbuf_p) {
        if ( (parm.doclip == 0) || 
            ( (*ib2_p > parm.clipmin) && (*ib2_p < parm.clipmax) ) ) {
          ax[nval] = *ib1_p;
          ay[nval] = *ib2_p;
          nval++;
        }
      }
      mbuf_p++;
      ib1_p++;
      ib2_p++;
    }
  }
  parm.nval = nval;

  if (parm.verbose)
    fprintf(stderr, "images read.  %ld points.  statrel...\n", parm.nval);

  (void) fpclose(parm.im1);
  (void) fpclose(parm.im2);

  free((addr_t) ibuf1);
  free((addr_t) ibuf2);
}
