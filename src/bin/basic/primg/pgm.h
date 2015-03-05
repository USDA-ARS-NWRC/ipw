#ifndef	PRIMG_H
#define	PRIMG_H

#include "IPWmacros.h"

typedef struct {
	int	geoband;    /* if > -1, use this band for pix coord system */
	double	bline;	    /* items from geo header */
	double	bsamp;
	double	dline;
	double  dsamp;
	double  xmin;	    /* image bounding box, in geo coords */
	double  ymin;
	double  xmax;
	double  ymax;
} PARM_T;
 
extern PARM_T   parm;

int             getcoords(int fd, int nlines, int nsamps, int *linep,
			  int *sampp);
void            primg(int fdi, int fdc, bool_t raw);

#endif
