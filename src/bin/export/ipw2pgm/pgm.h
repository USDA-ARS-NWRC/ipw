#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"
/*
 * header file for "ipw2pgm" program
 */


typedef struct {
  int        verbose;   /* should we talk back?         */
  int        im;	/* input image1 file descriptor	*/
  int        output;	/* output image file descriptor	*/
  int        imask;	/* mask image file descriptor	*/

  int        nlines;    /* lines                        */
  int        nsamps;    /* samples                      */
  int        nbands;    /* bands                        */
  int        nbits;     /* bits                         */
  int        maxn;      /* lines * samples              */

  pixel_t  * maskd;     /* mask data                    */
  pixel_t  * imd;       /* image data			*/

} PARM_T;

PARM_T parm;

extern void headers(void);
extern void ipw2pgm(void);
extern void qipw2pgm(void);

#endif
