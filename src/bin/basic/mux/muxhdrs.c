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

/*
 * muxhdrs -- multiplex image headers
 *
 * This function goes through some acrobatics to guarantee that all bands of
 * a given header are written CONTIGUOUSLY, as expected by the header ingest
 * functions. Specifically, we:
 *
 * 1) Read all headers from all input images and write consecutively to a
 *    single scratch file, changing the header band numbers to the
 *    corresponding output image band number.  As we read each header, we
 *    build a linked list of unique header names (i.e., if the header's name
 *    isn't already in the list, we add it).
 *
 * 2) For each header name in the list, read the entire scratch file, copying
 *    to the output image only those headers with the matching name.
 *
 * Since the entire scratch file is scanned (i.e. open-read-close) for each
 * unique header name, phase (2) above will clearly bog down if there are
 * lots of DIFFERENT kinds of headers.  One possible speed-up would be to
 * read the entire scratch file into memory once, then repeatedly scan that
 * chunk of memory.
 */

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "pgm.h"

typedef struct hlist {
	char           *name;		/* header name			 */
	struct hlist   *next;		/* -> next node in list		 */
}               HLIST_T;

void
muxhdrs(
	int             i_n,		/* # input files		 */
	int            *i_fdp,		/* -> input file descriptors	 */
	int            *i_nbandsp,	/* -> input # bands		 */
	int             o_fd)		/* output file descriptor	 */

{
	char           *hname;		/* current input header name	 */
	HLIST_T        *headp;		/* -> header name list		 */
	HLIST_T        *p;		/* -> current header name node	 */
	int             i;		/* current input image #	 */
	int             sum_nbands;	/* # output bands		 */
	int             scr_fd;		/* scratch file descriptor	 */
	char           *scr_name;	/* scratch file name		 */

 /*
  * construct scratch file name
  */
	scr_name = mktemplate(IPW_NAME);
	if (scr_name == NULL) {
		error("can't generate scratch file name");
	}

 /*
  * read all input headers
  */
	scr_fd = uwopen_temp(scr_name);
	if (scr_fd == ERROR) {
		error("can't open scratch file \"%s\"", scr_name);
	}

	headp = NULL;
	sum_nbands = 0;

	for (i = 0; i < i_n; ++i) {
		while ((hname = hrname(i_fdp[i])) != NULL
		       && strdiff(hname, BOIMAGE)) {
			HLIST_T        *tailp;	/* -> name list tail	 */

 /*
  * look for header name in name list
  */
			tailp = headp;
			for (p = headp; p != NULL; p = p->next) {
				if (streq(p->name, hname)) {
					break;
				}

				tailp = p;
			}

			if (p == NULL) {
 /*
  * append header name to name list
  */
				HLIST_T        *tmp_node;

 /* NOSTRICT */
				tmp_node = (HLIST_T *) ecalloc(1,
							   sizeof(HLIST_T));
				if (tmp_node == NULL) {
					error("can't allocate header name list node");
				}

				tmp_node->name = strdup(hname);
				if (tmp_node->name == NULL) {
					error(
					"can't duplicate header name \"%s\"",
					      hname);
				}

				if (headp == NULL) {
					headp = tmp_node;
				}
				else {
					tailp->next = tmp_node;
				}
			}
 /*
  * copy header to scratch file, changing band number to corresponding output
  * band number (i.e., += sum of previous input image bands)
  */
			if (hwprmb(scr_fd, hname,
				   hrband(i_fdp[i]) + sum_nbands,
				   hrvers(i_fdp[i]))
			    == ERROR) {
				error("\"%s\" header: can't write preamble",
				      hname);
			}

			if (hpass(i_fdp[i], scr_fd) == ERROR) {
				error("\"%s\" header: can't copy", hname);
			}
		}

		if (hname == NULL) {
			error("header read error");
		}

		sum_nbands += i_nbandsp[i];
	}
 /*
  * write an image preamble to mark end of scratch file
  */
	if (boimage(scr_fd) == ERROR) {
		error("can't terminate header output");
	}

	if (uclose(scr_fd) == ERROR) {
		error("can't close scratch file");
	}
 /*
  * walk header name list
  */
	for (p = headp; p != NULL; /* see end of loop */) {
		HLIST_T     *tmp_p;
 /*
  * scan scratch file
  */
		scr_fd = uropen(scr_name);
		if (scr_fd == ERROR) {
			error("can't re-open scratch file \"%s\" for reading",
			      scr_name);
		}

		while ((hname = hrname(scr_fd)) != NULL
		       && strdiff(hname, BOIMAGE)) {
 /*
  * if current header in scratch file matches current header name in the
  * list, then copy the header to the output image
  */
			if (streq(hname, p->name)) {
				if (hcopy(scr_fd, o_fd) == ERROR) {
					error(
					"\"%s\" header, band %d: can't copy",
					      hname, hrband(scr_fd));
				}
			}
			else {
				if (hrskip(scr_fd) == ERROR) {
					error(
					"\"%s\" header, band %d: can't skip",
					      hname, hrband(scr_fd));
				}
			}
		}

		if (uclose(scr_fd) == ERROR) {
			error("can't close scratch file");
		}

		tmp_p = p;
		p = p->next;
		SAFE_FREE(tmp_p->name);
		SAFE_FREE(tmp_p);
	}

	if (uremove(scr_name) == ERROR) {
		error("can't remove scratch file \"%s\"", scr_name);
	}
	SAFE_FREE(scr_name);

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}
}
