/*
** read stored profiles and write into output file
*/

#include <strings.h>

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

void
output(void)
{
	REG_1 int       nbytes;		/* # bytes in buffer		 */
	REG_2 int       nlines;		/* # lines			 */
	REG_3 int       nread;		/* # bytes to read		 */
	REG_4 DEM_SAVHDR *s;		/* saved part of profile hdr	 */
	REG_5 bool_t    rawflag;	/* ? (same as parm.raw)		 */
	REG_6 bool_t    fzero;		/* ? fill values are zeroes	 */
	addr_t          buf;		/* -> general I/O buffer	 */
	addr_t          rb;		/* -> either rfb or rpb		 */
	fpixel_t       *rfb;		/* -> floating buffer		 */
	int             (*elevwrite) ();/* -> write routine		 */
	int             fdt;		/* file descriptor		 */
	int             nsamps;		/* # samples			 */
	int             offset;		/* # pixels offset at start	 */
	pixel_t        *rpb;		/* -> raw buffer		 */

 /*
  * allocate I/O buffer
  */
	nsamps = hnsamps(parm.o_fd);
	rawflag = parm.raw;
	if (rawflag) {
		buf = ecalloc(nsamps, sizeof(pixel_t));
		nbytes = sizeof(pixel_t) * nsamps;
		elevwrite = pvwrite;
	}
	else {
		buf = ecalloc(nsamps, sizeof(fpixel_t));
		nbytes = sizeof(fpixel_t) * nsamps;
		elevwrite = fpvwrite;
	}
	assert(buf != NULL);
 /*
  * re-open temporary file
  */
	fdt = uropen(parm.ftemp);
	assert(fdt != ERROR);
 /*
  * saved parts of profile headers
  */
	s = parm.i_sav;
 /*
  * are fill values zeroes?
  */
	fzero = parm.fillval == 0;

	for (nlines = hnlines(parm.o_fd); --nlines >= 0;) {
 /*
  * Fill buffer, in case profile not complete.  The fill value will
  * always be zero if rawflag is set, hence no need to check.
  */
		if (fzero) {
			bzero(buf, nbytes);
		}
		else {
 /* NOSTRICT */
			fillvec((fpixel_t *) buf, parm.fillval, nsamps);
		}
 /*
  * profile offset at bottom
  */
		offset = (s->northing > parm.nmin) ?
			(s->northing - parm.nmin) / parm.dely : 0;
		if (rawflag) {
 /* NOSTRICT */
			rpb = (pixel_t *) buf;
			if (offset)
				rpb += offset;
 /* NOSTRICT */
			rb = (addr_t) rpb;
		}
		else {
 /* NOSTRICT */
			rfb = (fpixel_t *) buf;
			if (offset)
				rfb += offset;
 /* NOSTRICT */
			rb = (addr_t) rfb;
		}
		nread = s->nelev *
			(rawflag ? sizeof(pixel_t) : sizeof(fpixel_t));
		if (uread(fdt, rb, nread) != nread) {
			error("read error, profile %d", s->num);
		}
		if ((*elevwrite) (parm.o_fd, buf, nsamps) == ERROR) {
			error("write error, profile %d", s->num);
		}
		++s;
	}

 /*
  * clean up
  */
	(void) uclose(fdt);
	(void) unlink((char *) (parm.ftemp));
}
