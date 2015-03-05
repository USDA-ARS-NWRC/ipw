/*
** NAME
** 	output -- output image statistics
** 
** SYNOPSIS
**	#include "crh.h"
**
**	output (stat, abbrev, class_stats, cls_stat, nclass)
**	STAT      *stat;
**	bool_t     abbrev;
**	bool_t     class_stats;
**	CLS_STAT  *cls_stat;
**	int        nclass;
** 
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "crh.h"
#include "pgm.h"

void
output(
	STAT       stat,
	bool_t     abbrev,
	bool_t     print_units,
	bool_t     class_stats,
	CLS_STAT   cls_stat,
	int        nclass)
{
  int class;

  if (abbrev) {
    printf ("%ld ", stat.npixels);
    printf ("%.*g %.*g ", FLT_DIG, stat.min, FLT_DIG, stat.max);
    printf ("%.*g ", FLT_DIG, stat.median);
    printf ("%.*g %.*g", FLT_DIG, stat.mean, FLT_DIG, stat.sdev);

    if (print_units) {
	printf (" \"%s\"", stat.units);
    }

    printf ("\n");
  } else {
    printf ("npixels:\t%ld\n", stat.npixels);
    printf ("min:\t\t%.*g\n", FLT_DIG, stat.min);
    printf ("max:\t\t%.*g\n", FLT_DIG, stat.max);
    printf ("median:\t\t%.*g\n", FLT_DIG, stat.median);
    printf ("mean:\t\t%.*g\n", FLT_DIG, stat.mean);
    printf ("sdev:\t\t%.*g\n", FLT_DIG, stat.sdev);

    if (print_units) {
	printf ("units:\t\t%s\n", stat.units);
    }
  }

  if (class_stats) {
    if (abbrev) {
      printf ("%d\n", nclass);
    } else {
      printf ("\nnclasses: %d\n", nclass);
      printf ("class  #pixels           min         max      median        mean        sdev\n");
    }
    for (class=0; class < nclass; class++) {
      if (cls_stat.npixels[class] > 0) {
        printf ("%5d %8ld   ", class, cls_stat.npixels[class]);
        printf ("%11.*g ", FLT_DIG, cls_stat.min[class]);
        printf ("%11.*g ", FLT_DIG, cls_stat.max[class]);
        printf ("%11.*g ", FLT_DIG, cls_stat.median[class]);
        printf ("%11.*g ", FLT_DIG, cls_stat.mean[class]);
        printf ("%11.*g ", FLT_DIG, cls_stat.sdev[class]);
        printf ("\n");
      }
    }
  }

}
