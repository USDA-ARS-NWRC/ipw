/*
** Read through DEM profiles and write to temporary file.
** Keep track of boundaries as we go.
*/

#include "ipw.h"
#include "pgm.h"

void
profiles(void)
{
	DEM_SAVHDR     *s;		/* -> saved part of hdr	 */
	PROF_HDR       *ph =NULL;	/* -> profile header	 */
	REG_2 int       n;		/* index 		 */
	REG_3 bool_t    datum;		/* ? add elevation datum */
	addr_t          buf;		/* -> fbuf or pbuf	 */
	double          ntop;		/* northing top of prof	 */
	fpixel_t       *fbuf =NULL;	/* float I/O buffer	 */
	int             bufsize;	/* # elements in buffer	 */
	int             fdt;		/* file desc for ftemp	 */
	int             j;		/* index 		 */
	pixel_t        *pbuf =NULL;	/* raw I/O buffer	 */

 /*
  * Allocate I/O buffer.  At this time we don't know lengths of
  * profiles, but assume that maximum is 4 times the number of
  * profiles.
  */
	bufsize = 4 * parm.nprofls;

	if (parm.raw) {
 /* NOSTRICT */
		pbuf = (pixel_t *) ecalloc(bufsize, sizeof(pixel_t));
		assert(pbuf != NULL);
 /* NOSTRICT */
		buf = (addr_t) pbuf;
	}
	else {
 /* NOSTRICT */
		fbuf = (fpixel_t *) ecalloc(bufsize, sizeof(fpixel_t));
		assert(fbuf != NULL);
 /* NOSTRICT */
		buf = (addr_t) fbuf;
	}
 /*
  * allocate buffer to save parts of profile headers
  */
 /* NOSTRICT */
	s = parm.i_sav = (DEM_SAVHDR *) ecalloc(parm.nprofls,
						sizeof(DEM_SAVHDR));
	assert(parm.i_sav != NULL);
 /*
  * file to store elevation profiles
  */
	parm.ftemp = mktemplate("dem");
	assert(parm.ftemp != NULL);
	fdt = uwopen_temp(parm.ftemp);
	assert(fdt != ERROR);
	parm.maxelevs = 0;

	for (j = 0; j < parm.nprofls; ++j) {
 /*
  * read profile header
  */
		ph = profhdr(s);
		datum = ph->elevdatum != 0;
 /*
  * # of elevations in this profile
  */
		n = s->nelev;
		if (n > parm.maxelevs) {
			parm.maxelevs = n;
		}
 /*
  * boundaries for final output
  */
		if (s->northing < parm.nmin) {
			parm.nmin = s->northing;
		}
		if ((ntop = s->northing + parm.dely * (n - 1)) > parm.nmax) {
			parm.nmax = ntop;
		}
		if (s->easting < parm.emin) {
			parm.emin = s->easting;
		}
		else if (s->easting > parm.emax) {
			parm.emax = s->easting;
		}

 /*
  * Read the elevations in this profile. If 'raw' flag set, convert
  * directly to integer elevations, otherwise to float.
  */
		if (parm.raw) {
			REG_1 pixel_t  *pb;	/* -> pbuf		 */

			pb = pbuf;
			while (--n >= 0) {
				if (fscanf(parm.i_fp, "%u", pb) != 1) {
					warn("profile %d, unexpected EOF",
					     s->num);
					break;
				}
				if (datum) {
					*pb += ph->elevdatum + 0.5;
				}
				++pb;
			}
		}
		else {
			REG_1 fpixel_t *pfb;	/* -> fbuf		 */

			pfb = fbuf;
			while (--n >= 0) {
				if (fscanf(parm.i_fp, "%f", pfb) != 1) {
					warn("profile %d, unexpected EOF",
					     s->num);
					break;
				}
				if (datum) {
					*pfb += ph->elevdatum;
				}
				++pfb;
			}
		}
 /*
  * write to temporary output file
  */
		n = s->nelev * sizeof(fpixel_t);
		if (uwrite(fdt, buf, n) != n) {
			error("temporary write failed");
		}
 /*
  * if verbose option, print profile header on every NPRINT'th pass
  */
		if (parm.verbose && ((s->num % NPRINT) == 0 || j == 0)) {
			prprofhdr(ph);
		}
		++s;
	}
 /*
  * if verbose option, print last profile
  */
	if (parm.verbose) {
		prprofhdr(ph);
	}
 /*
  * clean up
  */
	free(buf);
	(void) uclose(fdt);
	(void) fclose(parm.i_fp);
}
