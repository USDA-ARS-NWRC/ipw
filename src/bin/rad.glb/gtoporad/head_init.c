/*
** NAME
**	head_init -- read headers, and write some
**
** DESCRIPTION
**	Reads BIH and LQH, writes output BIH, passes other headers
**	through.  (We can't write the output LQH until we process the
**	data.)
**
** RETURN VALUE
**	OK for normal return, ERROR otherwise.
**
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "gethdrs.h"
#include "lqh.h"
#include "sunh.h"
#include "pgm.h"

int
head_init (
	int             fdi,		/* input file descriptor	 */
	int             fdo,		/* output file descriptor	 */
	int             nbits)		/* # bits in output file	 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	int             j;

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH */

	if ((i_bih = bihread (fdi)) == NULL) {
		usrerr ("can't read BIH");
		return (ERROR);
	}

   /* correct # bands in input file? */

	if (hnbands (fdi) != NBANDS) {
		usrerr ("input file must have %d bands", NBANDS);
		return (ERROR);
	}

   /* output # bits is max # from input */

	if (nbits == 0) {
		for (j = 0; j < NBANDS; ++j) {
			if (hnbits (fdi, j) > nbits) {
				nbits = hnbits (fdi, j);
			}
		}
	}

   /* create output BIH */

	o_bih = (BIH_T **)
		hdralloc(1, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL) {
		usrerr ("Can't make new BIH");
		return (ERROR);
	}
	o_bih[0] = bihmake (0, nbits,
			   bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			   1);

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		usrerr ("Can't write output BIH");
		return (ERROR);
	}

   /* ingest LQH; other headers get passed thru */

	gethdrs (fdi, request, bih_nbands(o_bih[0]), fdo);
	if (!got_hdr(h_lqh)) {
		warn ("input image has no LQH");
	}

	return (OK);
}
