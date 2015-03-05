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
**	_cvt_opt_args -- type-convert option arguments
**
** SYNOPSIS
**	#include "_ipwmain.h"
**
**	bool_t
**	_cvt_opt_args(
**	    OPTION_T       *opt,	|* -> option descriptor		  *|
**	    char 	   *optargs)	|* string with option's arguments *|
**
** DESCRIPTION
**	_cvt_opt_args converts the arguments for an option into the appropriate
**	type (int, long, real, or str).  The arguments are supplied as a
**	comma-separated list in a single string.  This function parses the
**	string into the separate arguments, and then converts each argument
**	into the expected data type.  The # of arguments is also checked.
**
** RETURN VALUE
**	TRUE	The conversion of all the arguments was successful.
**
**	FALSE	A conversion error occurred; a explanatory message has been
**		saved using the "usrerr" function.
**
** GLOBAL VARIABLES READ
**
** GLOBAL VARIABLES MODIFIED
**
*/

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "ipw.h"
#include "_ipwmain.h"

bool_t
_cvt_opt_args(
	OPTION_T       *opt,		/* -> option descriptor		  */
	char 	       *optargs)	/* string with option's arguments */
{
	int       	n_args;		/* # of arguments 		 */
	int      	i;		/* loop index	 		 */
	char           *p;		/* temporary char pointer 	 */
	char          **args;		/* array of option's arguments	 */

	aint_t         *aint_p;		/* -> integer option-argument	 */
	along_t        *along_p;	/* -> long int option-argument	 */
	areal_t        *areal_p;	/* -> real option-argument	 */
	astr_t         *astr_p;		/* -> string option-argument	 */

	assert(opt != NULL);
	assert(optargs != NULL);

 /*
  * count individual option arguments (only if commas are not allowed in
  * option's argument)
  */
	n_args = 1;
	if (! opt->commasInArg) {
	    for (p = optargs; *p != EOS; p++) {
		if (*p == ',')
		    n_args++;
	    }
	}

 /*
  * make copies of individual arguments
  */
	args = (char **) ecalloc(n_args, sizeof(char *));
	if (args == NULL) {
		usrerr("insufficient memory to process option's arguments");
		return FALSE;
	}

	p = optargs;
	for (i = 0; i < n_args; i++) {
	    char * start;		/* start of current argument  */
	    int    len;			/* length of current argument */

	    /*
	     *  Scan for end of current argument: either EOS or ','
	     */
	    for (start = p, len = 0;
			(*p != EOS) && ((*p != ',') || opt->commasInArg); p++)
		len++;
	    p++;

	    args[i] = (char *) ecalloc(len + 1, sizeof(char));
	    if (args[i] == NULL) {
		usrerr("insufficient memory to process option's arguments");
		return FALSE;
	    }
	    (void) strncpy(args[i], start, len);
	    args[i][len] = EOS;
	}

 /*
  * too few ipwoptargs? (min_nargs == 0 means don't care)
  */
	if (opt->min_nargs > 0) {
		if (n_args < opt->min_nargs) {
			usrerr("missing argument(s) for option \"-%c\"",
			       opt->option);
			return FALSE;
		}
	}
 /*
  * too many ipwoptargs? (max_nargs == 0 means don't care)
  */
	if (opt->max_nargs > 0) {
		assert(opt->max_nargs >= opt->min_nargs);

		if (n_args > opt->max_nargs) {
			usrerr("too many arguments for option \"-%c\"",
			       opt->option);
			return FALSE;
		}
	}

	opt->nargs = n_args;
 /*
  * allocate space for type-converted arguments; do type conversion
  */
	switch (opt->type) {

	case STR_OPTARGS:
 /* NOSTRICT */
		astr_p = (astr_t *) ecalloc(n_args, sizeof(astr_t));
		if (astr_p == NULL) {
		    usrerr("insufficient memory to process option's arguments");
		    return FALSE;
		}
		(opt->args).astr_p = astr_p;

		for (i = 0; i < n_args; i++) {
		    *astr_p++ = (astr_t) (args[i]);
		}

		break;

	case INT_OPTARGS:
 /* NOSTRICT */
		aint_p = (aint_t *) ecalloc(n_args, sizeof(aint_t));
		if (aint_p == NULL) {
		    usrerr("insufficient memory to process option's arguments");
		    return FALSE;
		}
		(opt->args).aint_p = aint_p;

		errno = 0;
		for (i = 0; i < n_args; i++) {
		    aint_p[i] = (aint_t) (strtol(args[i], &p, 10));
		    if (errno != 0) {
			syserr();
			usrerr("integer argument out of range for option -%c",
			       opt->option);
			return FALSE;
		    }
		    if (*p != EOS) {
			usrerr("invalid integer argument \"%s\" for option -%c",
			       args[i], opt->option);
			return FALSE;
		    }
		}

		break;

	case LONG_OPTARGS:
 /* NOSTRICT */
		along_p = (along_t *) ecalloc(n_args, sizeof(along_t));
		if (along_p == NULL) {
		    usrerr("insufficient memory to process option's arguments");
		    return FALSE;
		}
		(opt->args).along_p = along_p;

		errno = 0;
		for (i = 0; i < n_args; i++) {
		    along_p[i] = (along_t) (strtol(args[i], &p, 10));
		    if (errno != 0) {
			syserr();
			usrerr("long int argument out of range for option -%c",
			       opt->option);
			return FALSE;
		    }
		    if (*p != EOS) {
			usrerr("invalid long int argument \"%s\" for option -%c",
			       args[i], opt->option);
			return FALSE;
		    }
		}

		break;

	case REAL_OPTARGS:
 /* NOSTRICT */
		areal_p = (areal_t *) ecalloc(n_args, sizeof(areal_t));
		if (areal_p == NULL) {
		    usrerr("insufficient memory to process option's arguments");
		    return FALSE;
		}
		(opt->args).areal_p = areal_p;

		errno = 0;
		for (i = 0; i < n_args; i++) {
		    areal_p[i] = (areal_t) (strtod(args[i], &p));
		    if (errno != 0) {
			syserr();
			usrerr("real argument out of range for option -%c",
			       opt->option);
			return FALSE;
		    }
		    if (*p != EOS) {
			usrerr("invalid real argument \"%s\" for option -%c",
			       args[i], opt->option);
			return FALSE;
		    }
		}

		break;

	default:
		bug("bad option type");
	}  /* switch */

 /*
  * if any type other than STR_OPTARGS, then dispatch local copies of arguments
  */
	if (opt->type != STR_OPTARGS) {
		for (i = 0; i < n_args; i++)
			free(args[i]);
	}
	free(args);

	return TRUE;
}
