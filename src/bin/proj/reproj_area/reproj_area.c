/*
** NAME
** 	reproj_area -- reproject masked image to get minimum output area
** 
** SYNOPSIS
**	void
**	reproj_area (fdi, bline, bsamp, dline, dsamp, iproj, oproj)
**	int fdi;
**	double bline, bsamp, dline, dsamp;
**	struct proj *iproj;
**	struct proj *oproj;
** 
** DESCRIPTION
**	reproj_area reads the input image, reprojects all unmasked points
**	keeping track of the minimum and maximum easting and northing.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "gctp.h"
#include "pgm.h"

void
reproj_area (
	int             fdi,		/* input image file descriptor	 */
	double		bline,		/* input image UL geo line coord */
	double		bsamp,		/* input image UL geo samp coord */
	double		dline,		/* input image geo line incr	 */
	double		dsamp,		/* input image geo samp incr	 */
	struct projdef *iproj,		/* input projection parms	 */
	struct projdef *oproj)		/* output projection parms	 */
{
	int		nlines;		/* # lines in input image	 */
	int		nsamps;		/* # samples in input image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		indef[3];	/* input proj ID/zone/units	 */
	int		outdef[3];	/* output proj ID/zone/units	 */
	int		errflg;		/* error return from gtrnz0	 */
	bool_t		first_pix;	/* flag for first unmasked pixel */
	double		incoord[2];	/* x,y coords for input image	 */
	double		outcoord[2];	/* x,y coords for output image	 */
	double		emin =0.0;	/* min easting in output space	 */
	double		emax =0.0;	/* max easting in output space	 */
	double		nmin =0.0;	/* min northing in output space	 */
	double		nmax =0.0;	/* max northing in output space	 */
	pixel_t        *ibuf;		/* input line buffer 		 */
	pixel_t        *ibufp;		/* -> in input buffer		 */
	FILE	       *errfile;


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);

   /* initialize projection definition arrays */

	indef[0] = iproj->id;
	indef[1] = iproj->zone;
	indef[2] = iproj->uid;
	outdef[0] = oproj->id;
	outdef[1] = oproj->zone;
	outdef[2] = oproj->uid;
	errfile = NULL;

   /* allocate buffer for input line */

	ibuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("error allocating input buffer");
	}


   /* loop on input image lines and samples */
   /* reproject to get output projection coordinates */

	incoord[0] = bsamp;
	incoord[1] = bline;
	first_pix = TRUE;

	for (line = 0; line < nlines; line++) {

		/* read input line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line);
		}

		ibufp = ibuf;

		for (samp = 0; samp < nsamps; samp++) {

			if (*ibufp++) {

				/* call GCTP to transform coordinates */

				gtrnz0_ (incoord, indef, iproj->parms, outcoord,
				 	outdef, oproj->parms, errfile, &errflg);

   				/* process error if any */
	
				if (errflg != 0) {
					error ("error %d reprojecting line %d sample %d",
						errflg, line, samp);
				}

				/* Update min & max coordinates */

				if (!first_pix) {
					emin = MIN (emin, outcoord[0]);
					emax = MAX (emax, outcoord[0]);
					nmin = MIN (nmin, outcoord[1]);
					nmax = MAX (nmax, outcoord[1]);
				} else {
					first_pix = FALSE;
					emin = emax = outcoord[0];
					nmin = nmax = outcoord[1];
				}
			}

			incoord[0] += dsamp;
		}

		incoord[0] = bsamp;
		incoord[1] += dline;
	}

   /* print rectangle */

	printf ("%f %f %f %f\n", emin, emax, nmin, nmax);
}
