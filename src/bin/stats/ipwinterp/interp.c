#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "pgm.h"

void
interp(double *poly)
{
  fpixel_t     * ibuf1;		/* image 1 input vector		*/
  fpixel_t     * ibuf_p;	/* pointer to image 1 		*/
  pixel_t      * mbuf;		/* entire mask image		*/
  pixel_t      * mbuf_p;	/* pointer to mask image	*/
  fpixel_t     * obuf;		/* output image			*/
  fpixel_t     * obuf_p;	/* pointer to output image	*/
  BIH_T       ** o_bihpp;       /* -> output BIH array           */
  LQH_T       ** o_lqhpp;       /* -> output LQH array           */
  pixel_t        ival[2];       /* min, max pixel values         */
  fpixel_t       fval[2];       /* min, max float values         */
  fpixel_t       tx, ty;
  int            j;
  long           line, samp;
  long           nval = 0;
  long           nextrap = 0;
  long           d = parm.degree;


        /* read mask */

  mbuf = 0;
  if (parm.maskfile != ERROR) {
    mbuf = (pixel_t *) ecalloc (parm.maxn, sizeof(pixel_t));
    if (mbuf == NULL) {
      error ("unable to allocate mask buffer");
    }
    if (pvread (parm.maskfile, mbuf, parm.maxn) != parm.maxn) {
      error ("error reading mask image");
    }
  }

        /* allocate image input buffer */

  ibuf1 = (fpixel_t *) ecalloc (parm.nsamps, sizeof (fpixel_t));
  if (ibuf1 == NULL) {
    error ("unable to allocate input buffer for image");
  }

        /* allocate image output buffer */

  obuf = (fpixel_t *) ecalloc (parm.maxn, sizeof(fpixel_t));
  if (obuf == NULL) {
    error ("unable to allocate output buffer");
  }


        /* read image */

  if (parm.verbose)
    fprintf(stderr, "reading image...max %ld points\n", parm.maxn);

  mbuf_p = mbuf;
  obuf_p = obuf;
  pstat.ymin =  1E38;
  pstat.ymax = -1E38;

  for (line = 0; line < parm.nlines; line++ ) {
    if ( fpvread (parm.im1, ibuf1, parm.nsamps) != parm.nsamps )
      error ("Error reading input image, line %d", line);

    ibuf_p = ibuf1;

    for (samp = 0; samp < parm.nsamps; samp++) {
      if (parm.maskfile == ERROR || *mbuf_p) {
        tx = *ibuf_p;
        ty = (fpixel_t) poly[d];
        for (j = d-1; j >= 0; j--)
         ty = tx * ty + (fpixel_t) poly[j];
        if (ty < pstat.ymin)   pstat.ymin = ty;
        if (ty > pstat.ymax)   pstat.ymax = ty;
        if ( (tx < pstat.xmin) || (tx > pstat.xmax) )
          nextrap++;
        nval++;
        *obuf_p = ty;
      } else
        *obuf_p = 0.0;     /* This isn't really right... */
      obuf_p++;
      mbuf_p++;
      ibuf_p++;
    }
  }

  if (parm.verbose) {
    fprintf(stderr, "%ld points.  %ld interpolations, %ld extrapolations.\n",
                     nval, nval - nextrap, nextrap);
  }

  (void) fpclose(parm.im1);

  free((addr_t) ibuf1);
  free((addr_t) mbuf);


        /* create and write output BIH */

  o_bihpp = (BIH_T **) hdralloc(1, sizeof(BIH_T *), parm.output, BIH_HNAME);

  assert(o_bihpp != NULL);

  o_bihpp[0] = bihmake(0, 
                       parm.nbits,
                       (STRVEC_T *) NULL,
                       parm.annot,
                       o_bihpp[0],
                       parm.nlines,
                       parm.nsamps,
                       1);

  assert(o_bihpp[0] != NULL);

  if (bihwrite(parm.output, o_bihpp) == ERROR) {
    error("can't write basic image header");
  }

        /* make and write the output LQH */

  ival[0] = 0;
  ival[1] = ipow2(parm.nbits) - 1;

  fval[0] = pstat.ymin;
  fval[1] = pstat.ymin == pstat.ymax ? pstat.ymin + 1.0 : pstat.ymax;

  o_lqhpp = (LQH_T **) hdralloc(1, sizeof(LQH_T *), parm.output, LQH_HNAME);

  o_lqhpp[0] = lqhmake(parm.nbits,
                       2,
                       ival,
                       fval,
                       (char *) NULL,
                       (char *) NULL);

  if (lqhwrite(parm.output, o_lqhpp) == ERROR) {
    error( "can't write LQH");
  }

  if (boimage(parm.output) == ERROR) {
    error( "can't terminate header output");
  }

        /* write the image */
        /* todo: write line by line to improve error messages */

  if (parm.verbose) 
    fprintf( stderr, "Writing image...\n");

  if (fpvwrite(parm.output, obuf, parm.maxn) != parm.maxn) {
    error( "error writing output image");
  }

  (void) fpclose( parm.output );
  
  free((addr_t) obuf);
}
