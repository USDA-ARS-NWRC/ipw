/*
 *  A simple "main" to build a program to test the token module.
 *  The program simply scans standard input for tokens, and prints
 *  the token types to standard output.
 */

#include "ipw.h"
#include "pgm.h"
#include "token.h"

char * ordinal(int i);

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
	    inputStr = str_arg(opt_expr, 0);
	    tokenInit(TRUE);
	}

	/*
	 *  Otherwise, initialize input file to standard input.
	 */
	else {
	    if (STREQ(str_arg(opt_file, 0), "-")) {
		inputFile = stdin;
	    } else {
	        inputFile = fopen(str_arg(opt_file, 0), "r");
	        if (inputFile == NULL) {
		    error("Cannot open the expression file: %s",
		          str_arg(opt_file, 0));
	        }
	    }
	    tokenInit(FALSE);
	}

	while (getToken())
	    {
	    if (got_opt(opt_file))
	        printf("Line %d: ", lineNum);
	    printf("%s", typeAsString(tokenType));
	    switch (tokenType) {
		case TOKEN_IDENTIFIER:
		    printf(" '%s'", tokenText);
		    break;
		case TOKEN_INTEGER:
		case TOKEN_BAND_AND_NUM:
		    printf(" %d", tokenInteger);
		    break;
		case TOKEN_FLOAT:
		    printf(" %g", tokenFloat);
		    break;
	    }
	    printf("\n");
	    if (tokenType == TOKEN_EOI)
		goto endOfInput;
	    }

	/*
	 *  Error happened.
	 */
	if (isprint(*tokenText))
	    error("Error in expression at the %d%s character ('%c') on line %d",
		  (tokenPosition + 1), ordinal(tokenPosition+1), *tokenText,
		  lineNum);
	else
	    error("Error in expression at the %d%s character on line %d",
		  (tokenPosition + 1), ordinal(tokenPosition+1), lineNum);

   endOfInput:
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
