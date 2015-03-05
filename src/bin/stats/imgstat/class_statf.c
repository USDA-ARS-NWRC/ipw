/*
** NAME
** 	class_statf -- fast image statistics per class
** 
** SYNOPSIS
**	#include "crh.h"
**
**	class_statf (fdi, fdm, stat, cls_stat, nclass, hist, hfp)
**	int fdi, fdm;
**	STAT *stat;
**	CLS_STAT  *cls_stat;
**	int nclass;
**	bool_t hist;
**	FILE *hfp;
** 
** DESCRIPTION
** 	class_statf reads the input image and computes the class statistics.
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
class_statf(
	int		fdi,		/* input image file desc	*/
	int		fdm,		/* mask  image file desc	*/
	STAT           *stat,
	CLS_STAT       *cls_stat,
	int             nclass,
	bool_t          hist,
	FILE           *hfp)
{
  int         line;
  int         samp;
  int         i;
  int         class;
  long        n;
  int         nbands;
  int         nlines;
  int         nsamps;
  int         nbits;
  int         nbins;
  double      fval;
  double      sigmax;
  double      sigmax2;
  long       *bin;
  long      **cls_bin;
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
  nbits  = hnbits (fdi, 1);


  /* Set up fpmap -- use band 1, as that is the data */

  fpmapv = fpmap(fdi)[1];

  /* Allocate bins */

  nbins = 1;
  for (i = 0; i < nbits; i++) {
    nbins *= 2;
  }
  if ( (bin = (long *) ecalloc (nbins, sizeof(long))) == NULL ) {
    error ("can't allocate bin space");
  }


  /* Allocate class stats arrays */

  if ((cls_stat->min = (fpixel_t *) ecalloc (nclass,sizeof(fpixel_t))) == NULL)
    error ("can't allocate class min array");
  if ((cls_stat->max = (fpixel_t *) ecalloc (nclass,sizeof(fpixel_t))) == NULL)
    error ("can't allocate class max array");
  if ((cls_stat->sigma = (double *) ecalloc (nclass,sizeof(double))) == NULL)
    error ("can't allocate class sigma array");
  if ((cls_stat->sigma2 = (double *) ecalloc (nclass,sizeof(double))) == NULL)
    error ("can't allocate class sigma2 array");
  if ((cls_stat->median = (fpixel_t *) ecalloc(nclass,sizeof(fpixel_t)))==NULL)
    error ("can't allocate class median array");
  if ((cls_stat->mean = (double *) ecalloc (nclass,sizeof(double))) == NULL)
    error ("can't allocate class mean array");
  if ((cls_stat->sdev = (double *) ecalloc (nclass,sizeof(double))) == NULL)
    error ("can't allocate class sdev array");
  if ((cls_stat->npixels = (long *) ecalloc (nclass,sizeof(long))) == NULL)
    error ("can't allocate class npixels array");

  /* Allocate class bin data */

  if ( (cls_bin = (long **) allocnd (sizeof(long), 2, nclass, nbins)) == NULL) {
    error ("can't allocate class bin space");
  }


  /* Allocate buffers */

  if ( (ibuf = (pixel_t *) ecalloc (nsamps * 2, sizeof(pixel_t))) == NULL ) {
    error ("can't allocate input buffer");
  }
  if (fdm != ERROR) {
    if ( (mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t))) == NULL ) {
      error ("can't allocate mask buffer");
    }
  } else {
    mbuf = (pixel_t *) NULL;
  }


  /* Accumulate data in bins */

  for (class = 0; class < nclass; class++) {
    for (i = 0; i < nbins; i++) {
      cls_bin[class][i] = 0;
    }
  }

#define IN_CLASS   *(ip)
#define IN_VALUE   *(ip+1)

  for (line = 0; line < nlines; line++) {
    if (pvread(fdi, ibuf, nsamps) != nsamps) {
      error ("Image read error, line %d", line);
    }
    if (fdm != ERROR) {
      if (pvread(fdm, mbuf, nsamps) != nsamps) {
        error ("Mask read error, line %d", line);
      }
    }
    ip = ibuf;
    mp = mbuf;
    for (samp = 0; samp < nsamps; samp++) {
      if (fdm == ERROR || *mp++) {
        cls_bin[IN_CLASS][IN_VALUE]++;
        cls_stat->npixels[IN_CLASS]++;
        bin[IN_VALUE]++;
        stat->npixels++;
      }
      ip += 2;
    }
  }

  if (fdm != ERROR) free(mbuf);
  free(ibuf);


  /* Calculate image statistics */

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
        if ( n > (stat->npixels / 2) ) {
          got_median = 1;
          stat->median = fval;
        }
      }
    }
  }
  stat->mean = sigmax / stat->npixels;
  stat->sdev = sqrt(( sigmax2 - ((sigmax*sigmax)/stat->npixels) )/stat->npixels);


  /* Calculate class statistics */

  for (class = 0; class < nclass; class++) {
    if (cls_stat->npixels[class] == 0) {
      cls_stat->min[class] = 0.0;
      cls_stat->max[class] = 0.0;
      cls_stat->mean[class] = 0.0;
      cls_stat->sdev[class] = 0.0;
      cls_stat->sigma[class] = 0.0;
      cls_stat->sigma2[class] = 0.0;
      cls_stat->median[class] = 0.0;
    } else {
      n = 0;
      cls_stat->sigma[class] = 0;
      cls_stat->sigma2[class] = 0;
      got_median = 0;
      got_min = 0;

      for (i = 0; i < nbins; i++) {
        if (cls_bin[class][i] > 0) {
        fval = fpmapv[i];
          cls_stat->sigma[class] += fval * cls_bin[class][i];
          cls_stat->sigma2[class] += (fval*fval) * cls_bin[class][i];
          if (!got_min) {
            got_min = 1;
            cls_stat->min[class] = fval;
          }
          cls_stat->max[class] = fval;
          if (!got_median) {
            n += cls_bin[class][i];
            if ( n > (cls_stat->npixels[class] / 2) ) {
              got_median = 1;
              cls_stat->median[class] = fval;
            }
          }
          if (hist) {
            fprintf (hfp, "%5d %11.*g %8ld\n", class, FLT_DIG,
                          fval, cls_bin[class][i]);
          }
        }
      }
      cls_stat->mean[class] = cls_stat->sigma[class] / cls_stat->npixels[class];
      cls_stat->sdev[class] =
                  sqrt( ( cls_stat->sigma2[class]
                          - (   (cls_stat->sigma[class]*cls_stat->sigma[class])
                              / cls_stat->npixels[class]
                            )
                        ) / cls_stat->npixels[class]
                      );
    }
  }


  free(bin);

}
