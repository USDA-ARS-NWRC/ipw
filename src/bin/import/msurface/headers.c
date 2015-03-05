/*
** NAME
** 	headers  -- read input header/write output images headers for surface
** 
** SYNOPSIS
**	void
**	headers (fdi, fdm, fdo, min, max, bline, bsamp, dline, dsamp)
**	int fdi, fdm, fdo;
**	float min, max;
**	double *bline, *bsamp, *dline, *dsamp;
** 
** DESCRIPTION
**	Headers reads/ingests/checks the input header and mask image
**	headers	and creates/writes the headers of the output image for the
**	program	surface.
** 
*/

#include <string.h>

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "pgm.h"

void
headers (
	int             fdi,		/* input file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output image file descriptors */
	float		min,		/* min value of point data	 */
	float		max,		/* max value of point data	 */
	double	       *bline,		/* line-coord of UL image corner */
	double	       *bsamp,		/* samp-coord of UL image corner */
	double	       *dline,		/* line spacing			 */
	double	       *dsamp,		/* sample spacing		 */
	int	        nclosest,       /* # neibs desired  	         */ 
 	int		obits,		/* # bits in output image 	 */
	int	       *inbands,	/* # bands in input img          */
 	FILE	       *ptfile,		/* input point file              */
	int 		npoints)  	/* # points in point file        */
{
	int		nbands;		/* # bands in images		 */
	BIH_T         **i_bih;		/* -> BIH of input image	 */
	LQH_T         **i_lqh;		/* -> LQH of input image	 */
	GEOH_T        **i_geoh;		/* -> GEOH of input image	 */
	BIH_T         **m_bih;		/* -> mask BIH			 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	LQH_T         **o_lqh;		/* -> output LQH	 	 */
	GEOH_T        **o_geoh;		/* -> output GEOH	 	 */
	pixel_t		ibkpt[2];	/* integer break points		 */
	fpixel_t	fbkpt[2];	/* FP break points		 */
	STRVEC_T       *annotp;		/* pointer to hdr annotation     */
	char            north[BUFSIZE]; /* string copies of coords for   */
	char            east[BUFSIZE];  /*  comparing to input pt hdr    */
 	int		i;

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, &h_geoh, 0};

/*&&& HARD-WIRED */
nbands = 1; /* # of output bands */

   /* read input header BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read input BIH");
	}

	*inbands = bih_nbands (i_bih[0]);

	if (nclosest > *inbands) {
	    error("requested # points exceeds # bands in input image");
	}

   /* read mask headers */
   /* make sure it has same dimensions as input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]) ||
       	    	    bih_nlines(i_bih[0]) != bih_nlines(m_bih[0])) {
		      error ("input and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* compare input point values with those in the input image header */

	annotp = bih_annot(i_bih[0]);

	rewind(ptfile);

	/* start at 1 to skip 1st annot line, which is descriptive text */

	for (i = 1; i <= npoints; i++) {

		if ( fscanf(ptfile, "%s%s%*s", east, north) != 2 )
		    error("bad read from point file at line %d",i);

		strcat(east, " ");	/* paste coords together */
		strcat(east, north);
	
		if ((annotp->v[i] == NULL) ||
		    (strcmp(east, annotp->v[i])) != 0 ) {
		      fprintf(stderr,"ERROR: input image and point file do "); 
		      fprintf(stderr,"not reference the same set of points.\n");
		      fprintf(stderr," input image says: %s\n", annotp->v[i]); 
		      fprintf(stderr," point file  says: %s\n", east); 
		      error("");
		}
	}

   /* create output BIH (do not copy annotation from input image */

	o_bih = (BIH_T **) hdralloc (nbands, sizeof(BIH_T *), fdo, BIH_HNAME);

	o_bih[0] = bihmake (0, obits,
			    bih_history(i_bih[0]), (STRVEC_T *) NULL,
			    o_bih[0], bih_nlines(i_bih[0]),
			    bih_nsamps(i_bih[0]), nbands);

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest input LQH and GEOH; other headers are passed through */

	gethdrs (fdi, request, nbands, fdo);

   /* make sure input header has geo header */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) == NULL) {
		error ("input header has no geodetic header");
	}

	*bline = geoh_bline (i_geoh[0]);
	*bsamp = geoh_bsamp (i_geoh[0]);
	*dline = geoh_dline (i_geoh[0]);
	*dsamp = geoh_dsamp (i_geoh[0]);

   /* warn if -g not used with "deg" units */
 
        if (streqn(geoh_units(i_geoh[0]), "deg", 3) && !parm.geoflag) {
            warn("Using Euclidean distance with 'deg' units");
        }
 
   /* warn if -g used with big-numbered coords */
 
        if(parm.geoflag &&
          ( (*bline < -90.0 || *bline > 90.0) ||
            (*bsamp < -180.0 || *bsamp > 180))   ) {
                warn ("Using geo distance with seemingly non-geo units.");
        }
 
   /* check for increasing northings */

	if (*dline > 0.0) {
	     warn ("geo hdr: dline is negative - northings should DECREASE");
	}

   /* create and write geo header for output image */

	o_geoh = geohdup (i_geoh, nbands);

	if (geohwrite (fdo, o_geoh) == ERROR) {
		error ("Can't write GEOH to output image");
	}

   /* duplicate input image LQH for output image */
   /* or create new header from min & max of point data */

	if ((i_lqh = (LQH_T **) hdr_addr (h_lqh)) != NULL) {


		o_lqh = (LQH_T **) lqhdup (i_lqh, nbands);
		if (o_lqh == NULL) {
			error ("Can't duplicate LQH of input image");
		}
	} else {
		/* allocate LQH */

		if ((o_lqh = (LQH_T **) hdralloc (nbands, sizeof(LQH_T *), fdo,
					     LQH_HNAME)) == NULL) {
			error ("can't allocate LQH header");
		}

		/* create LQH */

		ibkpt[0] = 0;
		ibkpt[1] = ipow2 (hnbits(fdo, 0)) - 1;
		fbkpt[0] = min;
		fbkpt[1] = max;
	        if(fbkpt[1] == fbkpt[0]) fbkpt[1]++;
		if ((o_lqh[0] = lqhmake (hnbits(fdo, 0), 2,
			ibkpt, fbkpt, (char *) NULL, (char *) NULL)) == NULL) {
			error ("can't make LQH");
		}
	}


   /* write LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for image output */

	if (boimage(fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
