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

/* LINTLIBRARY */

#include "ipw.h"
#include "_ipwmain.h"
#include "../../libipw/uio/_uio.h"	/* for _uioinit() */
#include "../../libipw/hdrio/_hdrio.h"	/* for _hdrioinit() */
#include "../../libipw/bih/_bih.h"	/* for _bihinit() */
#include "../../libipw/pixio/_pixio.h"	/* for _pixioinit() */
#include "../../libipw/fpio/_fpio.h"	/* for _fpioinit() */
#include "../../libipw/lqh/_lqh.h"	/* for _lqhinit() */
#include "../../libipw/crh/_crh.h"	/* for _crhinit() */

/*
** NAME
**	ipwenter -- initialize an IPW main program
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	ipwenter(argc, argv, optv, descrip)
**		int             argc,	   |* argc (from main()) 	 *|
**		char          **argv,	   |* argv (from main()) 	 *|
**		OPTION_T      **optv,	   |* -> option descriptors	 *|
**		const char     *descrip)   |* program description string *|
**
** DESCRIPTION
**	ipwenter should be the first executable statement in an IPW program's
**	main function.  Its chief function is to parse the command-line
**	arguments in argv according to the option descriptions supplied
**	in optv.  It also performs any necessary initialization of the IPW
**	subsystems (e.g., for IPW libraries).
**
**	optv is usually a static array, which is in turn initialized with
**	the addresses of static option descriptors, one per possible option.
**	For example:
**
**		main(argc, argv)
**			int		argc;
**			char		*argv[];
**		{
**
**	"-K" option; no arguments
**
**			static OPTION_T	opt_K = {
**				"K",
**				"temperatures are in degrees K instead of C",
**			};
**
**	"-bands" option; >= 3 integer arguments
**
**			static OPTION_T	opt_bands = {
**				"bands",
**				"bands to extract"
**				INT_OPTARGS,
**				"band",
**				OPTIONAL,
**				3,
**			};
**
**	"-images" option; >= 1 string argument; must be specified
**
**			static OPTION_T	opt_images = {
**				"images",
**				"images to process",
**				STR_OPTARGS,
**				"image",
**				REQUIRED,
**			};
**
**	"-datetime" option; >= 3 and <= 6 integer arguments; must be specified
**
**			static OPTION_T	opt_datetime = {
**			       "datetime",
**			       "starting date and time (default time: midnite)",
**			       INT_OPTARGS,
**			       "year,month,day,hr,min,sec",
**			       REQUIRED,
**			       3,
**			       6
**			};
**
**	operand; the "placeholder" is the single word placed on the synopsis
**	line in the usage message.  {REQUIRED,OPTIONAL} and the {min,max}
**	number of "arguments" (i.e. operands) are interpreted analogously to
**	options.
**
**	This example describes the most common situation, 0 or more string
**	operands (e.g. file names).
**
**			static OPTION_T operands = {
**				OPERAND,
**				"(operand description)",
**				STR_OPERANDS,
**				"(operand placeholder)",
**				OPTIONAL
**			};
**
**	vector of option descriptors; must be 0-terminated.  Operand
**	descriptors must FOLLOW all option descriptors.
**
**			static OPTION_T	*optv[] = {
**				&opt_K,
**				&opt_bands,
**				&opt_images,
**				&opt_datetime,
**				&operands,
**				0
**			};
**
**			ipwenter(argc, argv, optv, descrip);
**			...
**		}
**
** RETURN VALUE
**	None.  If ipwenter detects an error, it displays a diagnostic message
**	and terminates the program.
**
** GLOBALS ACCESSED
**
** HISTORY
**	Apr 1997	Moved initialization code for "o_byteorder" to
**			"../bih/_bihinit.c"  J. Domingo, OSU
**
**	Nov 1997	Modified this version of the routine to support
**			string options.  J. Domingo, OSU
*/

void
ipwenter(
	int             argc,		/* argc (from main())	 	 */
	char          **argv,		/* argv (from main())	 	 */
	OPTION_T      **optv,		/* -> option descriptors	 */
	const char     *descrip)	/* program description string	 */
{
	if (! _getargs(argc, argv, optv, descrip)) {
		usage();
	}

	_uioinit();
	_hdrioinit();
	_bihinit();
	_pixioinit();
	_fpioinit();
	_lqhinit();
	_crhinit();
}
