/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the Computer Systems Laboratory, University of
 * California, Santa Barbara and its contributors'' in the documentation
 * or other materials provided with the distribution and in all
 * advertising materials mentioning features or use of this software.
 *
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "pgm.h"

void
rmhdr(
	int             fdi,		/* input file descriptor	 */
	int             n_names,	/* # strings in "names"		 */
	char          **names,		/* -> array of header names	 */
	int             fdo)		/* output file descriptor	 */
{
	bool_t		copied;		/* ? header(s) copied to output	*/
	char           *hname;		/* current header name		 */

 /*
  * skip|copy headers
  */
	copied = FALSE;

	while ((hname = hrname(fdi)) != NULL && strdiff(hname, BOIMAGE)) {
		bool_t          skip;	/* ? skip : copy current header	 */

		if (n_names <= 0) {
 /*
  * no headers specified => skip all headers
  */
			skip = TRUE;
		}
		else {
 /*
  * skip only specified headers; copy all others to output
  */
			int             i;	/* loop counter		 */

			skip = FALSE;

			/*
			 *  Special case for basic image header, both the
			 *  per-image and per-band components.  Convert the
			 *  header name to generic "bi" header name.
			 */
			if (streq(hname, BIHI_HNAME)
						|| streq(hname, BIH_HNAME)) {
				hname = "bi";
			}

			for (i = 0; i < n_names; ++i) {
				if (streq(hname, names[i])) {
					skip = TRUE;
					break;
				}
			}
		}

		if (skip) {
			if (hrskip(fdi) == ERROR) {
				error("can't skip \"%s\" header", hname);
			}
		}
		else {
			if (hcopy(fdi, fdo) == ERROR) {
				error("can't copy \"%s\" header", hname);
			}

			copied = TRUE;
		}
	}

	if (hname == NULL) {
		error("header read error");
	}
 /*
  * if any headers were copied then write an image preamble
  */
	if (copied) {
		if (boimage(fdo) == ERROR) {
			error("can't terminate header output");
		}
	}
 /*
  * copy image data
  */
	if (ucopy(fdi, fdo, (long) LONG_MAX) == (long) ERROR) {
		error("image copy failed");
	}
}
