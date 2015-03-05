/*
** NAME
**      ipwload -- opens files and reads values into arrays
**
** DESCRIPTION
**      allocates mask buffer
**      allocates image buffer
**	allocates pixel bin storage
**	for each line
**	  read mask  data for this line
**	  read image data for this line
**	  for each sample
**	    increment the pixel bin of this line
**	close the image file
**	close the mask file
**	calculate mins, maxs, colors, etc.
**
** NOTES
**	Space is allocated for the entire image and the entire mask.  They
**	are not read in with one pvread however.  When doing a pvread, it
**	seems that IPW allocates space for nels + 2 * (nels*nsize), which
**	means if we read it all in at once, the image and mask would each
**	take over 3 times the storage required.  For large images this is
**	very bad.  We instead read a line at a time into the pre-allocated
**	space, which means pvread only allocates a small extra amount.
**
*/
#include "ipw.h"
#include "pgm.h"
#include "pixio.h"
#include "fpio.h"

void
ipwload(void)
{
  pixel_t     * ibp;		/* pointer to image  		*/
  pixel_t     * mbp;		/* pointer to mask image	*/
  long          line, samp;
  int           i;


        /* allocate mask buffers */

  if (parm.imask != ERROR) {
    parm.maskd = (pixel_t *) ecalloc (parm.maxn, sizeof(pixel_t));
    if (parm.maskd == NULL) {
      error ("Unable to allocate mask buffer");
    }
  } else
    parm.maskd = NULL;

        /* allocate image input buffers */

  parm.imd = (pixel_t *) ecalloc (parm.maxn, sizeof(pixel_t));
  if (parm.imd == NULL) {
    error ("Unable to allocate input buffer for image");
  }

  parm.pbins = (int *) ecalloc(ipow2(parm.nbits), sizeof(int));

  mbp = parm.maskd;
  ibp = parm.imd;

  for (line = 0; line < parm.nlines; line++) {
    if (parm.imask != ERROR)
      if (pvread (parm.imask, mbp, parm.nsamps) != parm.nsamps)
        error ("Error reading mask image, line %d", line);
    if (pvread (parm.im1, ibp, parm.nsamps) != parm.nsamps )
      error ("Error reading image data, line %d", line);
    for (samp = 0; samp < parm.nsamps; samp++) {
      if (parm.imask == ERROR || *mbp) {
        parm.pbins[*ibp]++;
      }
      mbp++;
      ibp++;
    }
  }

  parm.minp = 0;
  parm.maxp = 0;
  while (!parm.pbins[parm.minp])
    parm.minp++;
  parm.minv  = parm.fpm[parm.minp];

  parm.maxp = ipow2(parm.nbits) - 1;
  while (!parm.pbins[parm.maxp])
    parm.maxp--;
  parm.maxv  = parm.fpm[parm.maxp];

  parm.amaxv = MAX( ABS(parm.minv), ABS(parm.maxv) );
  parm.ucolors = 0;
  for (i = parm.minp; i <= parm.maxp; i++)
    if (parm.pbins[i])
      parm.ucolors++;

  if (parm.verbose)
    fprintf(stderr, "%d x %d x %d bit image with %d colors.  min: %f, max: %f\n\n",
                    parm.nlines, parm.nsamps, parm.nbits,
                    parm.ucolors, parm.minv, parm.maxv);
}
