/*
** NAME
** 	wbal -- run simple water balance model over input image
** 
** SYNOPSIS
**	void wbal (fdi, fdm, fdo)
**	int fdi;
**	int fdm;
**	int fdo;
** 
** DESCRIPTION
** 	wbal runs the simple water balance model on each point in the given
**	input image and writes the results to the output image.
** 
*/

#define		MAX_STOR	360.0
#define		S_INIT		0.10

#include	"ipw.h"
#include	"bih.h"
#include	"fpio.h"
#include	"pixio.h"
#include	"pgm.h"

void
wbal (
	int	fdi,		/* input image file descriptor		 */
	int	fdm,		/* mask image file descriptor		 */
	int	fdo)		/* output image file descriptor		 */
{
	float		avwat;		/* available water		 */
	float		ssat;		/* percent soil saturation	 */
	float		et;		/* scaled evapotranspiration	 */
	float		precip;		/* precipitation		 */
	float		pet;		/* PET				 */
	float		stor_init;	/* initial soil storage		 */
	float		stor_max;	/* maximum soil storage		 */
	float		stor;		/* new soil storage		 */
	float		runoff;		/* runoff			 */
	float		ro_min =0.0;	/* min runoff in output image	 */
	float		ro_max =0.0;	/* max runoff in output image	 */
	float		s_min =0.0;	/* min storage in output image	 */
	float		s_max =0.0;	/* max storage in output image	 */
	float		et_min =0.0;	/* min et in output image	 */
	float		et_max =0.0;	/* max et in output image	 */
	float		bflowf;		/* base flow factor		 */
	float		over_flow;	/* storage overflow		 */
	float		base_flow;	/* base flow 			 */
	fpixel_t       *ibuf;		/* pointer to input buffer	 */
	fpixel_t       *ibufp;		/* pointer in input buffer	 */
	fpixel_t       *obuf;		/* pointer to output buffer	 */
	fpixel_t       *obufp;		/* pointer in output buffer	 */
	pixel_t	       *mbuf;		/* pointer to mask buffer	 */
	pixel_t	       *mbufp;		/* pointer in mask buffer	 */
	int		nsamps;		/* # samples per line		 */
	int		nlines;		/* # lines in image		 */
	int		samp;		/* sample loop counter		 */
	int		line;		/* line loop counter		 */
	int		first_pixel;	/* flag for first pixel proc`ed  */
	int		fdt;		/* temp output file desc	 */
	int		nbytes;		/* #bytes to write		 */

	char	       *tempfile;	/* temporary output file	 */

	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;


   /* open temporary output file */

	tempfile = mktemplate("wbal");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (nsamps * IBANDS, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate input buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	first_pixel = TRUE;

	
   /* read input data and do calculations */

	for (line=0; line < nlines; line++) {

		/* read line of image */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("read error, line %d", line);
		}

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* Do calculations on each sample of line */

		ibufp = ibuf;
		mbufp = mbuf;
		obufp = obuf;

		for (samp=0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp++) {

				/* extract input bands */

				precip = *ibufp++;
				pet = *ibufp++;
				stor_init = *ibufp++;
				stor_max = *ibufp++;
				bflowf = *ibufp++;

				/*** temporary fix for stor_max==0.0 ***/

				if (stor_max <= 0.0) {
					stor_max = MAX_STOR;
					stor_init = S_INIT * stor_max;
				}

				/*** calculate runoff and change in storage ***/

				/* calculate available water, soil saturation */

				avwat = MAX (stor_init + precip, 0.0);
				ssat = avwat / stor_max;

				/* calc et, remove it from avwat, & adj ssat */

				et = aet (pet, ssat);
				if (avwat + et < 0.0)
					et = -avwat;
				avwat += et;
				ssat = avwat / stor_max;

				/* calc baseflow, over_flow, & runoff */

				base_flow = baseflow (bflowf, ssat);
				base_flow = MIN (avwat, MIN (base_flow, stor_max));
				over_flow = MAX(avwat - base_flow - stor_max, 0.0);
				runoff = over_flow + base_flow;

				/***	check for 0 runoff	***/

				if (runoff <= 0.0) {
					fprintf (stderr, "%d, %d, %g\n", line, samp, runoff);
				}

				ssat = avwat / stor_max;

				/* reset stor for next interation */

				stor = MAX (avwat - runoff, 0.0);

				/* set output bands */

				*obufp++ = runoff;
				*obufp++ = stor;
				*obufp++ = et;


				/* update min/max */

				if (!first_pixel) {
					ro_min = MIN (ro_min, runoff);
					ro_max = MAX (ro_max, runoff);
					s_min = MIN (s_min, stor);
					s_max = MAX (s_max, stor);
					et_min = MIN (et_min, et);
					et_max = MAX (et_max, et);
				} else {
					first_pixel = FALSE;
					ro_min = runoff;
					ro_max = runoff;
					s_min = stor;
					s_max = stor;
					et_min = et;
					et_max = et;
				}

			} else {
				*obufp++ = 0.0;
				*obufp++ = 0.0;
				*obufp++ = 0.0;
				ibufp += IBANDS;
			}
		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

   /* create/write LQH for output image */

	newlqh (fdo, ro_min, ro_max, s_min, s_max, et_min, et_max);

   /* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);

}
