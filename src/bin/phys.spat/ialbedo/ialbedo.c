/*
 ** NAME
 ** 	ialbedo -- read cosz input image and calculate albedo
 **
 ** SYNOPSIS
 **	ialbedo (fdi, fdo, start, day, gsize, maxgsz, dirt, vis_only, ir_only);
 **	int fdi, fdo;
 **	double start, day;
 **	double gsize, maxgsz, dirt;
 **	bool_t vis_only, ir_only;
 **
 ** DESCRIPTION
 **	ialbedo reads the input image, calculates solar albedo over snow
 **	and writes the output image for the ialbedo program.
 **
 */

#include <math.h>
#include "ipw.h"
#include "fpio.h"
#include "bih.h"
#include "pixio.h"
#include "pgm.h"

void
ialbedo (
		int		fdi,		/* input image file desc	 */
		int		fdo,		/* output image file desc	 */
		double		start,		/* day of last storm 		 */
		double		day,		/* current day			 */
		double		gsize,		/* initial grain radius 	 */
		double		maxgsz,		/* grain radius growth limit 	 */
		double		dirt,		/* visible contamination factor  */
		bool_t		vis_only,	/* output visible band only	 */
		bool_t		ir_only)	/* output IR band only		 */
{
	int		nsamps;		/* # samples in input image	 */
	int		nlines;		/* # lines in output image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		obands;		/* # output bands		 */
	int		maplen;		/* length of input image F.P map */
	int	       *fmaplen;	/* -> lengths of F.P maps	 */
	float		cosz;		/* cosine of zenith angle 	 */
	double		alb_ir_1;	/* IR albedo for cosz = 1	 */
	double		alb_v_1;	/* Visible albedo for cosz = 1	 */
	double		dzir;		/* ir diurnal increase range 	 */
	double		dzv;		/* visible diurnal increase rng	 */
	double		gir;		/* sqrt of eff ir grain radius	 */
	double		gv;		/* sqrt of ef vis grain radius	 */
	double		growth_factor;	/* grain growth factor 		 */
	double		radius_ir;	/* sqrt initial eff ir grain rad */
	double		radius_v;	/* sqrt init eff vis grain rad	 */
	double		range_ir;	/* rng of sqrt eff ir grain rad  */
	double		range_v;	/* rng of sqrt eff vis grain rad */
	pixel_t        *ibuf;		/* input line buffer		 */
	pixel_t        *ibufp;		/* -> in input line buffer	 */
	pixel_t		pixel;		/* input pixel value		 */
	fpixel_t	vmin =0.0;	/* min alb(vis) in output image */
	fpixel_t	vmax =0.0;	/* max alb(vis) in output image */
	fpixel_t	irmin =0.0;	/* min alb(ir) in output image	 */
	fpixel_t	irmax =0.0;	/* max alb(ir) in output image	 */
	fpixel_t       *obuf;		/* output line buffer		 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	fpixel_t      **fmap;		/* -> input img F.P. maps	 */
	fpixel_t       *map;		/* input image floating point map*/
	fpixel_t       *alb_ir;		/* IR albedo 			 */
	fpixel_t       *alb_v;		/* Visible albedo 		 */


	nsamps = hnsamps (fdi);
	nlines = hnlines (fdi);
	if (vis_only || ir_only)
		obands = 1;
	else
		obands = 2;

	/* Allocate input line buffer */

	ibuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

	/* Allocate output line buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * obands, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	/* Access input image floating point maps */

	fmap = fpmap (fdi);
	map = fmap[0];
	fmaplen = fpmaplen (fdi);
	maplen = fmaplen[0];

	/* Allocate alb(vis) and alb(ir) lookup table */

	alb_v = (fpixel_t *) ecalloc (maplen, sizeof(fpixel_t));
	if (alb_v == NULL) {
		error ("can't allocate albedo lookup table");
	}
	alb_ir = (fpixel_t *) ecalloc (maplen, sizeof(fpixel_t));
	if (alb_ir == NULL) {
		error ("can't allocate albedo lookup table");
	}

	/* set initial grain radii for vis and ir */

	radius_ir = sqrt (gsize);
	range_ir = sqrt (maxgsz) - radius_ir;
	radius_v = sqrt (dirt * gsize);
	range_v = sqrt (dirt * maxgsz) -radius_v;

	/* calc grain growth decay factor */

	growth_factor = growth ((day - start) + 1.0);

	/* calc effective gsizes for vis & ir */

	gv = radius_v + (range_v * growth_factor);
	gir = radius_ir + (range_ir * growth_factor);

	/* adjust diurnal increase range */

	dzv = gv * VZRG;
	dzir = (gir * IRZRG) + IRZ0;

	/* calc albedos for cos(z)=1 */

	alb_v_1 = MAXV - (gv / VFAC);
	alb_ir_1 = MAXIR * exp(IRFAC * gir);

	/* build lookup tables for albedo indexed on input image pixel value */

	for (pixel = 0; pixel < maplen; pixel++) {

		cosz = map[pixel];

		/* check to see if sun is up */

		if (cosz > 0.0) {

			/* calculate albedo */

			alb_v[pixel] = alb_v_1 + dzv * (1.0 - cosz);
			alb_ir[pixel] = alb_ir_1 + dzir * (1.0 - cosz);

		} else {
			alb_v[pixel] = alb_v_1;
			alb_ir[pixel] = alb_ir_1;
		}

		/* keep min and max of each output band */

		if (pixel > 0) {
			vmin = MIN (alb_v[pixel], vmin);
			vmax = MAX (alb_v[pixel], vmax);
			irmin = MIN (alb_ir[pixel], irmin);
			irmax = MAX (alb_ir[pixel], irmax);
		} else {
			vmin = alb_v[pixel];
			vmax = alb_v[pixel];
			irmin = alb_ir[pixel];
			irmax = alb_ir[pixel];
		}

	}

	/* create/write LQH for output image */

	newlqh (fdo, vmin, vmax, irmin, irmax, vis_only, ir_only);

	/* loop on image lines */

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line);
		}

		ibufp = ibuf;
		obufp = obuf;

		/* loop on samples - map through lookup tables */

		for (samp = 0; samp < nsamps; samp++) {

			pixel = *ibufp++;

			if (!ir_only)
				*obufp++ = alb_v[pixel];
			if (!vis_only)
				*obufp++ = alb_ir[pixel];
		}

		/* write output line buffer to output image */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("write error, line %d", line);
		}
	}

}
