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
**	_getargs -- get command-line options and arguments
**
** SYNOPSIS
**	#include "_ipwmain.h"
**
**	char **
**	getargs(
**	    int              argc,	|* # command-line argumnents	 *|
**	    char           **argv,	|* -> command-line arguments	 *|
**	    REG_2 OPTION_T **optv,	|* -> option descriptors	 *|
**	    const char      *descrip)	|* -> program descripton string	 *|
**
** DESCRIPTION
**	getargs fetches all options from the command-line arguments pointed to
**	by argv, according to the option descriptors pointed to by optv.  The
**	program description string "descrip" is saved externally for future
**	usage message generation.
**
** RETURN VALUE
**	getargs returns a pointer to the first command-line operand in argv,
**	or NULL if the command line is incorrect.
**
** GLOBAL VARIABLES MODIFIED
**	_argv
**	_descrip
**	_optv
**
** RESTRICTIONS
**	Non-string operands (i.e., values other than STR_OPERANDS) are not
**	supported.
*/

#include <string.h>
#include <unistd.h>

#include "ipw.h"
#include "_ipwmain.h"


char **
_getargs(
	int              argc,		/* # command-line argumnents	 */
	char           **argv,		/* -> command-line arguments	 */
	REG_2 OPTION_T **optv,		/* -> option descriptors	 */
	const char      *descrip)	/* -> program descripton string	 */
{

	bool_t          get_operands;	/* ? process operands		 */
	REG_1 int       i;		/* loop counter			 */
	REG_3 int       n_opts;		/* # option descriptors		 */
	REG_4 int       option;		/* current option		 */
	REG_6 char     *optstring;	/* legal option letters		 */
	REG_5 char     *optstr_p;	/* -> optstring			 */

	assert(argc > 0);
	assert(argv != NULL);

 /*
  * set globals
  */
	_argv = argv;
	_descrip = strdup(descrip);
	_optv = optv;

 /*
  * special case: "pgm -H" means print usage message
  */
	if (argc == 2 && streq(argv[1], "-H")) {
		return (NULL);
	}

 /*
  * turn off error messages from getopt()
  */
	opterr = 0;

	if (optv == NULL) {

 /*
  * trivial case: no options requested; so barf if any supplied
  */
		if (getopt(argc, argv, "") != -1) {
			usrerr("no options allowed");
			return (NULL);
		}

		return (&argv[optind]);
	}

 /*
  * count option requests
  */
	get_operands = FALSE;
	for (n_opts = 0; optv[n_opts] != 0; ++n_opts) {
		if (optv[n_opts]->option == OPERAND) {
			get_operands = TRUE;
			break;
		}
	}

	if (get_operands && n_opts < 1) {
		goto operands;
	}

 /*
  * allocate an "optstring" for getopt(): leave room for ':' suffixes, plus
  * leading ':' and EOS
  */
	optstring = (char *) ecalloc(2 * n_opts + 2, sizeof(char));
	if (optstring == NULL) {
		usrerr("can't allocate optstring for getopt()");
		return (NULL);
	}
	optstr_p = optstring;
	*optstr_p++ = ':';

 /*
  * load optstring with key-letters of requested options
  */
	for (i = 0; i < n_opts; ++i) {
		*optstr_p++ = optv[i]->option;

		if (optv[i]->type != NO_OPTARGS) {

 /*
  * add suffix indicating option with arguments
  */
			*optstr_p++ = ':';
		}
	}

	*optstr_p = EOS;

 /*
  * loop through command-line options
  */
	while ((option = getopt(argc, argv, optstring)) != -1) {

		if (option == '?') {
			usrerr("unknown option \"-%c\"", optopt);
			return (NULL);
		} else if (option == ':') {
			usrerr("missing argument(s) for option \"-%c\"",
				optopt);
			return (NULL);
		} else {

			 /*
			  * look for matching option request
			  */
			for (i = 0; i < n_opts; ++i) {
				if (optv[i]->option == option) {
					break;
				}
			}

			if (i >= n_opts) {
				bug("getopt returned unknown option");
			}

			/*
			 * found a match, now decode ipwoptargs
			 */
			if (optv[i]->type == NO_OPTARGS) {
				 /*
				  * no optargs, just note presence of option
				  */
				optv[i]->nargs = 1;
			}
			else if (! _cvt_opt_args(optv[i], optarg)) {
				return (NULL);
			}
		}  /* else */
	}  /* while */

 /*
  * make sure that all required options were supplied
  */
	for (i = 0; i < n_opts; ++i) {
		if (optv[i]->required
		    && optv[i]->nargs < 1
		    && optv[i]->option != OPERAND) {
			usrerr("required option \"-%c\" missing",
			       optv[i]->option);
			return (NULL);
		}
	}

	free(optstring);

operands:
 /*
  * process operand requests, if any
  * 
  * NB: should expand this someday to accommodate non-string operands
  */
	if (get_operands) {
		int             n_operands;	/* # operands	 */

		i = n_opts;

		if (optv[i]->type != STR_OPERANDS) {
			bug("only STR_OPERANDS allowed");
		}

		n_operands = argc - optind;

		if (optv[i]->required && n_operands < optv[i]->min_nargs) {
			usrerr("required operand(s) missing");
			return (NULL);
		}

		if (optv[i]->max_nargs > 0 &&
			    n_operands > optv[i]->max_nargs) {
			usrerr("too many operands");
			return (NULL);
		}

		optv[i]->nargs = n_operands;
		(optv[i]->args).astr_p = &argv[optind];
	}  /* if */

 /*
  * return pointer to first operand
  */
	return (&argv[optind]);
}
