/*
 *   A simple program to test the command interpreter.  If it successfully
 *   compiles the expression, it then executes the sequence of
 *   interpreter commands for the expression.  For execution,
 *   command-line operands are interpreted as band values:
 *
 *	operands[0] = band[0]
 *	operands[1] = band[1]
 *	operands[2] = band[2]
 *	...
 *	operands[nBands-1] = band[nBands-1]
 */

#include <ctype.h>
#include <stdlib.h>

#include "ipw.h"
#include "pgm.h"
#include "command.h"
#include "token.h"

char * ordinal(int i);

#define CMD_BUF_LEN	100

void
main(
	int   argc,
	char *argv[])
{
        static OPTION_T opt_expr = {
                "expr", "arithmetic expression",
                STR_OPTARGS, "string",
                OPTIONAL, ONE_ARG_WITH_COMMAS,
        };
 
        static OPTION_T opt_file = {
                "file", "file with arithmetic expression",
                STR_OPTARGS, "string",
                OPTIONAL, 1, 1,
        };

        static OPTION_T operands = {
                OPERAND, "band values",
                STR_OPERANDS, "band",
                OPTIONAL
        };

        static OPTION_T *optv[] = {
                &opt_expr,
                &opt_file,
                &operands,
                NULL
        };
 
	int	   nBands;
	fpixel_t  *bands;
	int	   i;
	command_t  buffer[CMD_BUF_LEN];
	int        nCmds;
	fpixel_t   result;

 
        /*
         *  Begin
         */
        ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 
	/*
	 *  Need one of the options.
	 */
	opt_check(1, 1, 2, &opt_expr, &opt_file);

	/*
	 *  Setup the command-line arguments as band values.
	 */
	nBands = n_opnds(operands);
	if (nBands > 0) {
	    bands = (fpixel_t *) ecalloc(nBands, sizeof(fpixel_t));
	    if (bands == NULL)
		error("Can't allocate memory for bands");
	    for (i = 0; i < nBands; i++)
		bands[i] = (fpixel_t) strtod(str_opnd(operands, i), NULL);
	} else {
	    bands = NULL;
	}

	/*
	 *  If we are given a command line argument, use it
	 *  as an input string.
	 */
	if (got_opt(opt_expr)) {
	    nCmds = compile(str_arg(opt_expr, 0), TRUE, buffer, CMD_BUF_LEN);
	}

	/*
	 *  Otherwise, initialize input file to standard input.
	 */
	else {
	    nCmds = compile(str_arg(opt_file, 0), FALSE, buffer, CMD_BUF_LEN);
	}

	if (nCmds > 0) {
	    if (execute(buffer, nCmds, bands, nBands, &result)) {
		 printf("%g\n", result);
		 exit(0);
	    } else {
		error("Execution error");
		exit (1);
	    }
	} else if (nCmds == 0) {
	    printf("  Error: empty expression\n");
	} else {

	    /*
	     *  Error happened.
	     */
	    if (tokenType == TOKEN_EOI)
		error("Error at the end of the expression");
	    else if (isprint(*tokenText))
		error("Error in expression at the %d%s character ('%c')",
			  (tokenPosition + 1), ordinal(tokenPosition+1),
			  *tokenText);
	    else
		error("Error in expression at the %d%s character",
			  (tokenPosition + 1), ordinal(tokenPosition+1));
	}
}

/* ------------------------------------------------------------------------ */

char *
ordinal(
	int i)

	{
	if ((i % 10 == 1) && (i % 100 != 11))
		return "st";
	else if ((i % 10 == 2) && (i % 100 != 12))
		return "nd";
	else if ((i % 10 == 3) && (i % 100 != 13))
		return "rd";
	else
		return "th";
	}
