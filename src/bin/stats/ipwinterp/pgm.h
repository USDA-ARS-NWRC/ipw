#include "IPWmacros.h"

#include "fpio.h"
#include "pixio.h"

/*
 * header file for "istatrel" program
 */

typedef struct {
  int        nfiles;    /* number of files               */
  long       needfiles; /* number of files needed        */
  int        im1;       /* input image1 file descriptor	 */
  int        output;    /* output image file descriptor	 */
  int        maskfile;  /* mask image file descriptor	 */
  int        nlines;    /* lines                         */
  int        nsamps;    /* samples                       */
  int        nbits;     /* bits in output image          */
  long       nval;      /* number of points (after mask and clip) */
  long       maxn;      /* maximum number of points      */
  long       degree;    /* degree of polynomial          */
  int        verbose;   /* ? verbose                     */
  int        analyze;   /* ? analyze statistics only     */
  STRVEC_T * annot;     /* annotation header from relat  */
} PARM_T;

/*
    S == Schaum's Outline Series: Theory and Problems of Statistics
    L == Applied Linear Regression Models.  Neter, Wasserman, & Kutner
*/

typedef struct {
  double  xmin,
          xmax,
          ymin,
          ymax,
          syx,          /* standard error of estimate   S 243  */
          sdev,         /* standard deviation           S  70  */
          d_r2,
          r2,           /* coefficient of multiple det  L 241  */
          d_cofv,
          cofv,         /* coefficient of variation     S  73  */
          cofc;         /* coefficient of contingency	S 204  */
} STAT_T;

STAT_T   pstat;
PARM_T   parm;

extern void     read_par(double *poly);
extern void     stat_anal(void);
extern void     headers(void);
extern void     interp(double *poly);
