#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "ipw2ppm" program
 */

#if __STDC__ == 1
#define linepr(xm) fprintf(stderr, "File: %s, Line: %d, Msg: %s\n", \
                           __FILE__, __LINE__, #xm)
#else
#define linepr(xm) fprintf(stderr, "File: %s, Line: %d, Msg: \"xm\"\n", \
                           __FILE__, __LINE__)
#endif

typedef struct {
  int     r;
  int     g;
  int     b;
} colors;

typedef struct {
  double   value;
  colors   c;
} cmentry;

typedef struct {
  int        verbose;   /* should we talk back?         */
  int        cbar;      /* add a colorbar?              */
  int        nfiles;	/* number of files              */
  int        im1;	/* input image1 file descriptor	*/
  int        output;	/* output image file descriptor	*/
  int        imask;	/* mask image file descriptor	*/
  FILE     * cmfile;	/* colormap file descriptor	*/
  int        graymap;   /* should we use a graymap      */
  int        forcepgm;  /* write a PGM file             */

  int        nlines;    /* lines                        */
  int        nsamps;    /* samples                      */
  int        nbands;    /* bands                        */
  int        nbits;     /* bits                         */
  int        nbits0;    /* bits in image 0              */
  int        nbits1;    /* bits in image 1              */
  int        nbits2;    /* bits in image 2              */
  int        maxn;      /* lines * samples              */

  pixel_t  * maskd;     /* mask data                    */
  pixel_t  * imd;       /* image data			*/
  int      * pbins;
  int        ucolors;
  colors   * CLUT;
  fpixel_t * fpm;
  double     minv;      /* minimum data value		*/
  double     maxv;      /* maximum data value		*/
  int        minp;	/* pixel value of min data value*/
  int        maxp;	/* pixel value of min data value*/
  double     amaxv;     /* absolute maximum data value	*/

  int        nD;
  cmentry    D[256];
  int        nP;
  cmentry    P[256];
  int        nM;
  cmentry    M;

  colors     gblack, gwhite, gred, gblue, ggreen;

} PARM_T;

PARM_T parm;

extern void	headers(void);
extern void	ipwload(void);
extern void	initcolors(void);
extern void	readcolormap(void);
extern void	pgmwrite(void);
extern void	ppmwrite(void);
extern void	ipw3ppm(void);
extern colors	rgblookup(int pix);
extern addr_t	ecalloc(int nelem, int elsize);

#endif
