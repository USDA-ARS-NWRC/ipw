/*
** NAME
** 	statf -- fast image statistics
** 
** SYNOPSIS
**	#include "crh.h"
**	#include "lqh.h"
**
**	statf (fdi, fdm, stat)
**	int fdi, fdm;
**	STAT *stat;
** 
** DESCRIPTION
** 	statf reads the input image and computes the statistics.
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "crh.h"
#include "pgm.h"

void
statf(
	int		fdi,		/* input image file desc	 */
	int		fdm,		/* mask image file desc		 */
	STAT           *stat)
{
  int         line;
  int         samp;
  int         i;
  long        n;
  int         nbands;
  int         nlines;
  int         nsamps;
  long        npixels;
  int         nbits;
  int         nbins;
  long       *bin;
  double      fval;
  double      sigmax;
  double      sigmax2;
  bool_t      got_median;
  bool_t      got_min;
  pixel_t    *ibuf;
  pixel_t    *mbuf;
  pixel_t    *ip;
  pixel_t    *mp;
  fpixel_t   *fpmapv;
  
  nbands = hnbands (fdi);
  nsamps = hnsamps (fdi);
  nlines = hnlines (fdi);
  nbits  = hnbits (fdi, 0);


  /* Set up fpmap */

  fpmapv = fpmap(fdi)[0];

  /* Allocate bins */

  nbins = 1;
  for (i = 0; i < nbits; i++) {
    nbins *= 2;
  }
  if ( (bin = (long *) ecalloc (nbins, sizeof(long))) == NULL ) {
    error ("can't allocate bin space");
  }


  /* Allocate buffers */

  if ( (ibuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t))) == NULL ) {
    error ("can't allocate input buffer");
  }


  /*
   * Accumulate data in bins.
   *
   * Seperate cases for mask and no mask.  Why?  Because we can shave off
   * a few tests.  We're trying to be fast, so we take what we can get.
   * This actually does save a little bit of time, but will only be noticable
   * on slow machines or very large images (bigger than 1000x1000).
   */

  if (fdm != ERROR) {
     if ( (mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t))) == NULL ) {
       error ("can't allocate mask buffer");
     }
     npixels = 0;
     for (line = 0; line < nlines; line++) {
       if (pvread(fdi, ibuf, nsamps) != nsamps) {
         error ("Image read error, line %d", line);
       }
       if (pvread(fdm, mbuf, nsamps) != nsamps) {
         error ("Mask read error, line %d", line);
       }
       ip = ibuf;
       mp = mbuf;
       for (samp = 0; samp < nsamps; samp++) {
         if (*mp++) {
           bin[*ip]++;
           npixels++;
         }
         ip++;
       }
     }
     free(mbuf);
  } else {
     npixels = nlines * nsamps;
     for (line = 0; line < nlines; line++) {
       if (pvread(fdi, ibuf, nsamps) != nsamps) {
         error ("Image read error, line %d", line);
       }
       ip = ibuf;
       for (samp = 0; samp < nsamps; samp++) {
         bin[*ip++]++;
       }
     }
  }
  free(ibuf);


  n = 0;
  sigmax = 0;
  sigmax2 = 0;
  got_median = 0;
  got_min = 0;

  for (i = 0; i < nbins; i++) {
    if (bin[i] > 0) {
      fval = fpmapv[i];
      sigmax += fval * bin[i];
      sigmax2 += (fval*fval) * bin[i];
      if (!got_min) {
        got_min = 1;
        stat->min = fval;
      }
      stat->max = fval;
      if (!got_median) {
        n += bin[i];
        if (n > (npixels/2)) {
          got_median = 1;
          stat->median = fval;
        }
      }
    }
  }

  /*
   * Reverse lq maps can make these backwards.
   */
  if (stat->min > stat->max) {
    fval = stat->min;
    stat->min = stat->max;
    stat->max = fval;
  }

  stat->mean = sigmax / npixels;
  stat->sdev = sqrt( ( sigmax2 - ((sigmax*sigmax)/npixels) ) / npixels );
  stat->npixels = npixels;

  free(bin);

}
