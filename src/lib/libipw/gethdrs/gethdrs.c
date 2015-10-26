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
 ** NAME
 **	gethdrs -- process IPW image headers
 **
 ** SYNOPSIS
 **	#include "gethdrs.h"
 **
 **	void
 **	gethdrs(
 **		int             fdi,	  |* input image file descriptor  *|
 **		GETHDR_T      **request,  |* -> header requests		  *|
 **		int             nbands,	  |* # bands in output image	  *|
 **		int             fdo)	  |* output image file descriptor *|
 **
 ** DESCRIPTION
 **	gethdrs provides a high-level interface to the IPW "hdrio" routines.
 **	Image headers are read from file descriptor fdi and, subject to the
 **	interpretation of request, written to file descriptor fdo.
 **
 **	request points to an array of pointers to GETHDR_T structures.  The
 **	last element in the array must be a NULL pointer.
 **
 **	Each GETHDR_T structure specifies the disposition of a particular
 **	type of IPW header.  The first two fields of the structure should be
 **	initialized to the name of the IPW header to be processed, and a
 **	pointer to the header's ingest routine.  If the ingest routine pointer
 **	is NULL, then the header will be skipped; otherwise, the header will
 **	be ingested and its address will be available via the hdr_addr macro
 **	when gethdrs returns.
 **
 **	Headers that are not mentioned in the structures pointed to via
 **	request, and that pertain to band numbers less than nbands, are
 **	copied to file descriptor fdo.  You must therefore read the BIH from
 **	fdi, and write a BIH to fdo, before calling gethdrs.
 **
 **	nbands == NO_COPY prevents any header output by gethdrs.
 **
 **	The following example will hopefully make this a little clearer:
 **
 **		static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t)lqhread};
 **		static GETHDR_T h_orh = {ORH_HNAME};
 **		static GETHDR_T *request[] = {&h_lqh, &h_orh, NULL};
 **		...
 **		i_bihpp = bihread(i_fd);
 **		...
 **		bihwrite(o_fd, o_bihpp);
 **		gethdrs(i_fd, request, bih_nbands(o_bihpp), o_fd);
 **		i_lqhpp = (LQH_T **) hdr_addr(h_lqh);
 **		...
 **		lqhwrite(o_fd, o_lqhpp);
 **		boimage(o_fd);
 **
 **	In this code fragment, we have arranged to ingest a LQ header, skip
 **	an ORH header, and copy any other headers encountered to fdo.  Note
 **	that we must explicity write a LQ header if we want one to be placed
 **	in the output image -- ingested headers are NOT copied.
 **
 ** ERRORS
 **	can't copy {header-name} header
 **	can't skip {header-name} header
 **	can't ingest {header-name} header
 **	header read error
 */

#include "ipw.h"
#include "hdrio.h"
#include "gethdrs.h"

void
gethdrs(
		int             fdi,		/* input image file descriptor	 */
		GETHDR_T      **request,	/* -> header requests		 */
		int             nbands,		/* # bands in output image	 */
		int             fdo)		/* output image file descriptor	 */
{
	REG_2 char     *hname;		/* name of current header	 */
	REG_1 GETHDR_T *p;		/* fast *request	 	 */

	while ((hname = hrname(fdi)) != NULL && strdiff(hname, BOIMAGE)) {
		/*
		 * look for request that matches current header name
		 */
		if (request == NULL) {
			p = NULL;
		}
		else {
			REG_3 GETHDR_T **pp;	/* fast request		 */

			for (pp = request;
					(p = *pp) != NULL && strdiff(p->name, hname);
					++pp) {
				continue;
			}
		}
		/*
		 * copy this header to output image
		 */
		if (p == NULL) {
			if (hrband(fdi) < nbands) {
				if (hcopy(fdi, fdo) == ERROR) {
					error("can't copy \"%s\" header",
							hname);
				}
			}
			else {
				if (hrskip(fdi) == ERROR) {
					error("can't skip \"%s\" header",
							hname);
				}
			}
		}
		/*
		 * skip this header
		 */
		else if (p->ingest == NULL) {
			if (hrskip(fdi) == ERROR) {
				error("can't skip \"%s\" header", hname);
			}
		}
		/*
		 * ingest this header
		 */
		else {
			p->hdr = (*p->ingest) (fdi);
			/*
			 * NB: can't use hname in error message: if the error occurred in the check()
			 * routine, hname will have already been set from the preamble of the next
			 * header.
			 */
			if (p->hdr == NULL) {
				error("can't ingest header");
			}
		}
	}

	if (hname == NULL) {
		error("header read error");
	}
}
