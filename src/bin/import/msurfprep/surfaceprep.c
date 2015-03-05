/****************************************************************************
SURFACEPREP - create an image of nearest neighbors for use with the
	msurface program. 
****************************************************************************/
#include "ipw.h"  
#include "pgm.h"
#include "pixio.h"

#define debugflag 0

void surfaceprep(
	int	lines,
	int	samps,
	int	nneibs,
	double	bline,
	double	bsamp,
	double	dline,
	double	dsamp,
	int	npoints,
	int	fdo) 
{
    int     i, j, k,
	    n_done;
    int     Compare();			/* passed to SORT_ALG */
    double  atof();
    double  cellx, celly;	        /* geodetic coords of a grid cell */
    pixel_t *obuf, *obufp;		/* output buffer, pointer */
    void    Find_neibs();

    /* allocate output buffer */

    obuf = (pixel_t *) ecalloc(samps * nneibs, sizeof(pixel_t));
    if(obuf == NULL) error("can't allocate pixel I/O buffer");
    obufp = obuf;

    celly = bline;
    cellx = bsamp;

    for(i=0; i<lines; i++) {	  /* loop over lines */

	if(i % 10 == 0) fprintf(stderr,"  %d lines to go\n", lines - i);


	obufp = obuf;

        for(j=0; j<samps; j++) {  /* loop over samps */

	    /* find closest neibs */

	    Find_neibs(nneibs, npoints, cellx, celly, i, j);

	    /* sort neibs by distance */

	    SORT_ALG(closest, nneibs, sizeof(NEIB), Compare);

            for(k=0; k<nneibs; k++) {  /* loop over nearest neibs */

	        /* write closest array to output buffer */

		obufp[k] = (pixel_t) closest[k].id;

	    }

	    obufp += nneibs;

  	    cellx += dsamp; 

	} /* end samp loop */

	/* write data to file */
	n_done = pvwrite(fdo, obuf, samps);
	if(n_done != samps) error("bad write to output image, line %d", i); 
	
        celly += dline;   
	cellx = bsamp;

    }     /* end line loop */

} /* end surfaceprep function */

int
Compare(
	NEIB	*elem1,
	NEIB	*elem2)      /* compare function which is passed to SORT_ALG */
{
        if (elem1->dist2 < elem2->dist2) return(-1);
        else if (elem1->dist2 > elem2->dist2) return(1);
        else return(0);
}

