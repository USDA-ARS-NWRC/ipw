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
**	bool_t
**	getargs(
**	    int              argc,	|* # command-line argumnents	 *|
**	    char           **argv,	|* -> command-line arguments	 *|
**	    REG_2 OPTION_T **optv,	|* -> option descriptors	 *|
**	    const char      *descrip)	|* -> program descripton string	 *|
**
** DESCRIPTION
**	_getargs fetches all options from the command-line arguments pointed to
**	by argv, according to the option descriptors pointed to by optv.  The
**	program description string "descrip" is saved externally for future
**	usage message generation.
**
** RETURN VALUE
**	TRUE	all command-line arguments processed
**
**	FALSE	error occurred, and an explanatory message stored via "usrerr"
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


bool_t
_getargs(
	int              argc,		/* # command-line argumnents	 */
	char           **argv,		/* -> command-line arguments	 */
	REG_2 OPTION_T **optv,		/* -> option descriptors	 */
	const char      *descrip)	/* -> program descripton string	 */
{

	bool_t          get_operands;	/* ? process operands		 */
	REG_2 int       i;		/* loop counter			 */
	REG_1 int       j;		/* loop counter			 */
	REG_3 int       n_opts;		/* # option descriptors		 */
	REG_4 char     *arg;		/* current command-line argument */
	REG_5 int	len;		/* length of 'arg'		 */
	REG_6 OPTION_T *match;		/* current matching option	 */

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
		return FALSE;
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
  * loop through command-line options
  */
	i = 1;
	while ((i < argc) && (argv[i][0] == '-')) {
		/*
		 *  The argument "--" indicates no more options in command-line
		 */
		if (strcmp(argv[i], "--") == 0) {
			i++;
			break;  /* exit 'for' loop */
		}

		/*
		 *  Truncate the leading "-" off argument for searching.
		 */
		arg = argv[i] + 1;
		len = strlen(arg);

		/*
		 *  Search for matching option request
		 */
		if (optv == NULL) {
			 /*
			  * trivial case: no options requested.
			  */
			usrerr("no options allowed");
			return FALSE;
		}

		match = NULL;
		for (j = 0; j < n_opts; ++j) {
			if (strncmp(arg, optv[j]->option, len) == 0) {
				if (match == NULL) {
					match = optv[j];
				} else {
					usrerr("ambiguous option \"-%s\"", arg);
					return FALSE;
				}
			}
		}

		if (match == NULL) {
			usrerr("unknown option \"-%s\"", arg);
			return FALSE;
		}


		/*
		 *  Process the option's arguments.
		 */
		i++;
		if (match->type == NO_OPTARGS) {
			 /*
			  * no optargs, just note presence of option
			  */
			match->nargs = 1;
		} else {
			if ((i >= argc) || (strcmp(argv[i], "--") == 0)) {
				usrerr("missing argument(s) for option \"-%s\"",
					match->option);
				return FALSE;
			}
			if (! _cvt_opt_args(match, argv[i])) {
				return FALSE;
			}
			i++;
		}  /* else */
	}  /* while */

 /*
  * make sure that all required options were supplied
  */
	for (j = 0; j < n_opts; ++j) {
		if (optv[j]->required
		    && optv[j]->nargs < 1
		    && optv[j]->option != OPERAND) {
			usrerr("required option \"-%s\" missing",
			       optv[j]->option);
			return FALSE;
		}
	}

operands:
 /*
  * process operand requests, if any
  * 
  * NB: should expand this someday to accommodate non-string operands
  */
	if (get_operands) {
		int             n_operands;	/* # operands	 */

		j = n_opts;

		if (optv[j]->type != STR_OPERANDS) {
			bug("only STR_OPERANDS allowed");
		}

		n_operands = argc - i;

		if (optv[j]->required && n_operands < optv[j]->min_nargs) {
			usrerr("required operand(s) missing");
			return FALSE;
		}

		if (optv[j]->max_nargs > 0 &&
			    n_operands > optv[j]->max_nargs) {
			usrerr("too many operands");
			return FALSE;
		}

		optv[j]->nargs = n_operands;
		(optv[j]->args).astr_p = &argv[i];
	}  /* if */

	return TRUE;
}
