#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

/*
 * despike -- remove abnormally large values from an image
 *
 * walk through the image, replacing spikes with averages
 */

void
despike(void)
{
  fpixel_t	* il1;			/* first  line of data		*/
  fpixel_t	* il2;			/* second line of data		*/
  fpixel_t	* il3;			/* third  line of data		*/
  fpixel_t	* il1_p;		/* pointers to those lines	*/
  fpixel_t	* il2_p;		/* pointers to those lines	*/
  fpixel_t	* il3_p;		/* pointers to those lines	*/
  fpixel_t	* itemp;		/* temporary pointer		*/
  pixel_t	* mbuf;			/* mask image			*/
  pixel_t	* mbuf_p;		/* pointer to mask image	*/
  fpixel_t	* obuf;			/* output image			*/
  fpixel_t	* obuf_p;		/* pointer to output image	*/
  long		  line, samp;		/* line and sample counters	*/
  double	  ave;

  il1 = (fpixel_t *) ecalloc(parm.nsamps+2, sizeof(fpixel_t));
  il2 = (fpixel_t *) ecalloc(parm.nsamps+2, sizeof(fpixel_t));
  il3 = (fpixel_t *) ecalloc(parm.nsamps+2, sizeof(fpixel_t));
  if ( (il1 == NULL) || (il2 == NULL) || (il3 == NULL) ) {
    error("can't allocate input buffers");
  }

  obuf = (fpixel_t *) ecalloc(parm.nsamps, sizeof(fpixel_t));
  if (obuf == NULL) {
    error("can't allocate output buffer");
  }

  if (parm.maskfile != ERROR) {
    mbuf = (pixel_t *) ecalloc (parm.nsamps, sizeof(pixel_t));
    if (mbuf == NULL) {
      error ("unable to allocate mask buffer");
    }
  } else {
    mbuf = NULL;
  }

  for (line = 0; line < parm.nlines; line++ ) {

    if (line == 0) {
      if (fpvread (parm.im1, il2+1, parm.nsamps) != parm.nsamps )
        error ("Error reading input image, line %d", line);
      *il2 = *(il2+1);
      *(il2+parm.nsamps+1) = *(il2+parm.nsamps);
      if (fpvread (parm.im1, il3+1, parm.nsamps) != parm.nsamps )
        error ("Error reading input image, line %d", line+1);
      *il3 = *(il3+1);
      *(il3+parm.nsamps+1) = *(il3+parm.nsamps);
      il1_p = il2;  il2_p = il2;  il3_p = il3;
    } else if (line == (parm.nlines-1) ) {
      il1_p = il2;  il2_p = il3;  il3_p = il3;
    } else {
      itemp = il1;
      il1 = il2;
      il2 = il3;
      il3 = itemp;
      if (fpvread (parm.im1, il3+1, parm.nsamps) != parm.nsamps )
        error ("Error reading input image, line %d", line+1);
      *il3 = *(il3+1);
      *(il3+parm.nsamps+1) = *(il3+parm.nsamps);
      il1_p = il1; il2_p = il2; il3_p = il3;
    }
    if ( (parm.maskfile != ERROR) && (line != parm.nlines) ) {
      if (pvread (parm.maskfile, mbuf, parm.nsamps) != parm.nsamps)
        error ("error reading mask image");
    }

    mbuf_p = mbuf;
    obuf_p = obuf;
    for (samp = 0; samp < parm.nsamps; samp++) {
      il1_p++;  il2_p++;  il3_p++;
      if (parm.maskfile == ERROR || *mbuf_p) {
        if (*il2_p >= parm.lowpass) {
          ave = *(il1_p-1) + *il1_p + *(il1_p+1) +
                *(il2_p-1) +          *(il2_p+1) +
                *(il3_p-1) + *il3_p + *(il3_p+1);
          ave /= 8;
          if ( *il2_p > (ave * parm.threshmult) ) {
            *obuf_p = ave;
          } else {
            *obuf_p = *il2_p;
          }
        } else {
          *obuf_p = *il2_p;
        }
      } else {
        *obuf_p = *il2_p;
      }
      mbuf_p++;
      obuf_p++;
    }

    if (fpvwrite(parm.outfile, obuf, parm.nsamps) != parm.nsamps) {
      error( "error writing output image");
    }

  }

  (void) fpclose(parm.outfile);

  SAFE_FREE(il1);
  SAFE_FREE(il2);
  SAFE_FREE(il3);
  free((addr_t) obuf);
  if (parm.maskfile != ERROR) {
    free((addr_t) mbuf);
  }
}
