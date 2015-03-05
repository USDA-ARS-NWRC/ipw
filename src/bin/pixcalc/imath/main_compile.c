/*
 *   A simple program to test the compiler.
 *
 *   If the program successfully compiles an expression, it then prints
 *   the sequence of interpreter commands for the expression to standard
 *   output.
 */

#include <ctype.h>

#include "ipw.h"
#include "pgm.h"
#include "compile.h"
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

        static OPTION_T *optv[] = {
                &opt_expr,
                &opt_file,
                NULL
        };
 
	command_t  buffer[CMD_BUF_LEN];
	int        nCmds;
	command_t *cmd;

 
        /*
         *  Begin
         */
        ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 
	/*
	 *  Need one of the options.
	 */
	opt_check(1, 1, 2, &opt_expr, &opt_file);

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
	    /*
	     *  Print the command buffer.
	     */
	    for (cmd = buffer; nCmds > 0; nCmds--, cmd++) {
		printf("  ");
		printCmd(cmd);
		printf("\n");
	    }
	    printf ("  Constant expression: %s\n", constantExpr ? "yes"
								    : "no");
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
