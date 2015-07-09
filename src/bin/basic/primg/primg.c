#include "ipw.h"

#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pixio.h"
#include "geoh.h"
#include "lqh.h"

#include "pgm.h"

static addr_t   buf;			/* input buffer			 */
static int      nbands;			/* # bands / image sample	 */

static void
dprint(
		int             samp)
{
	REG_2 int       band;
	REG_1 pixel_t  *pixel;
	REG_3 int       lastband;

	/* NOSTRICT */
	pixel = (pixel_t *) buf;
	pixel += samp * nbands;

	lastband = nbands - 1;

	for (band = 0; band < nbands; ++band) {
		(void) printf("%u%c", *pixel++, band == lastband ? '\n' : ' ');
	}
}

static void
gprint(
		int             samp)
{
	REG_2 int       band;
	REG_1 fpixel_t *fpixel;
	REG_3 int       lastband;

	/* NOSTRICT */
	fpixel = (fpixel_t *) buf;
	fpixel += samp * nbands;

	lastband = nbands - 1;

	for (band = 0; band < nbands; ++band) {
		(void) printf("%.*g%c", FLT_DIG,
				*fpixel++, band == lastband ? '\n' : ' ');
	}
}

static void
naprint(void)
{
	REG_2 int       band;
	REG_3 int       lastband;

	lastband = nbands - 1;

	for (band = 0; band < nbands; ++band) {
		(void) printf("%s%c", "na", band == lastband ? '\n' : ' ');
	}
}

/*
 * primg -- print image pixels
 */

void
primg(
		int             fdi,		/* input image file descriptor	 */
		int             fdc,		/* coordinate file descriptor	 */
		bool_t          raw)		/* ? print raw values		 */
{
	BIH_T         **bihpp;		/* -> BI header array		 */
	int             cline;		/* line # of next pixel to print */
	int             csamp;		/* samp # of next pixel to print */
	int             line;		/* current image line #		 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / line		 */
	int             (*in) ();	/* -> image read function	 */
	void            (*out) ();	/* -> pixel output function	 */
	int             pixsize;	/* sizeof input pixel		 */
	GEOH_T        **geohpp; 	/* -> geo header         	 */
	LQH_T         **lqhpp; 		/* -> lq header         	 */
	double		temp1, temp2;	/* temporary coord values	 */
	int		status = OK;	/* ret. value of getcoords() 	 */

	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T h_lq  = {LQH_HNAME,  (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_geo, &h_lq, 0};

	/*
	 * read BIH
	 */
	bihpp = bihread(fdi);
	if (bihpp == NULL) {
		error("can't read basic image header");
	}

	nbands = hnbands(fdi);
	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);

	/* read geo and lq headers */

	gethdrs (fdi, request, NO_COPY, ERROR);

	/*
	 * if requested, do stuff with geo header and values
	 */
	if (parm.geoband >= 0) {
		if (parm.geoband > (nbands - 1)) {
			error("requested geo band > nbands");
		}

		if ((geohpp = (GEOH_T **) hdr_addr(h_geo)) == NULL) {
			error("no geo header");
		}
		if (geohpp[parm.geoband] == NULL) {
			error("requested band has a missing geo header");
		}
		parm.bline = geoh_bline (geohpp[parm.geoband]);
		parm.bsamp = geoh_bsamp (geohpp[parm.geoband]);
		parm.dline = geoh_dline (geohpp[parm.geoband]);
		parm.dsamp = geoh_dsamp (geohpp[parm.geoband]);

		/* compute bounding box */
		temp1 = parm.bsamp - (parm.dsamp / 2.0);
		temp2 = temp1 + (nsamps * parm.dsamp);
		parm.xmin = MIN(temp1, temp2);
		parm.xmax = MAX(temp1, temp2);

		temp1 = parm.bline - (parm.dline / 2.0);
		temp2 = temp1 + (nlines * parm.dline);
		parm.ymin = MIN(temp1, temp2);
		parm.ymax = MAX(temp1, temp2);
	}

	/*
	 * initialize for raw or floating-point pixels
	 */
	if (raw) {
		skiphdrs(fdi);
		pixsize = sizeof(pixel_t);
		in = pvread;
		out = dprint;
	}
	else {
		if ((lqhpp = (LQH_T **) hdr_addr(h_lq)) == NULL) {
			error("no lq header");
		}

		pixsize = sizeof(fpixel_t);
		in = fpvread;
		out = gprint;
	}

	buf = ecalloc(nsamps * nbands, pixsize);
	if (buf == NULL) {
		error("can't allocate image input buffer");
	}

	/*
	 * read 1st coordinate pair
	 */
	if (fdc != ERROR) {
		status = getcoords(fdc, nlines, nsamps, &cline, &csamp);

		if (status == EOF)  { 
			return; 
		}
		else if (status == ERROR) {
			csamp = -1;
			cline = 0;  /* this prevents premature return */
		}
	}
	/*
	 * read image lines
	 */
	for (line = 0; line < nlines; ++line) {
		if ((*in) (fdi, buf, nsamps) != nsamps) {
			error("image read error, line %d", line);
		}

		/* fdc == ERROR means print everything */

		if (fdc == ERROR) {
			int             samp;	/* current image sample # */

			for (samp = 0; samp < nsamps; ++samp) {
				(*out) (samp);
			}
		}

		/*
		 * otherwise, only print as long as this line 
		 * matches current coords
		 */

		else {
			while (cline == line) { 

				/* use naprint if point is off image */

				if (cline >= nlines || cline < 0 ||
						csamp >= nsamps || csamp < 0 ||
						status == ERROR)  {
					naprint();
				} else {
					(*out) (csamp);
				}

				status = getcoords(fdc, nlines, nsamps,
						&cline, &csamp);

				if (status == EOF)  { return; }
			}
		}
	}
}
