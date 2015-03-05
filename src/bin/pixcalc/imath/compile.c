/*
 *  Module for compiler.
 */
 
/* ------------------------------------------------------------------------ */

/*
** NAME
**      compile -- compile an expression into interpreter commands
**
** SYNOPSIS
**	#include "compile.h"
**
**	int
**	compile(
**	    char      * str,	    |* expression or name of file with it  *|
**	    bool_t      strIsExpr,  |* TRUE if str has expression;
**				       FALSE if str is file name           *|
**	    command_t * buffer,	    |* buffer for interpreter commands	   *|
**	    int	        length)	    |* length of buffer		  	   *|
**
** DESCRIPTION
**	This routine compiles an expression into a sequence of
**	interpreter commands.  The expression is either in the given
**	string, or is read from the specified file.  The interpreter
**	commands are placed into the given buffer.
**
** RETURN VALUE
**
**	> 0	# of commands put into the command buffer
**
**	0	No expression was found.
**
**	-1	An error occured, and a message explaining the error
**		has been stored with the "usrerr" routine.
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
**	inputFile
**	inputStr
*/

#include <string.h>

#include "ipw.h"
#include "pgm.h"
#include "compile.h"
#include "token.h"
#include "cmdbuf.h"
 
/* ------------------------------------------------------------------------ */

/*
 *  Global variables.
 */

bool_t constantExpr = FALSE;
	/*
	 *  Was the expression reduced to a constant expression?
	 */
 
/* ------------------------------------------------------------------------ */

int
compile(
	char      * str,	/* expression or name of file with it  */
	bool_t      strIsExpr,	/* TRUE if str has expression;
				   FALSE if str is file name	       */
	command_t * buffer,	/* buffer for interpreter commands     */
	int	    length)	/* length of buffer		       */	
{
	assert(str != NULL);

	/*
	 *  Initialize the command-buffer module.
	 */
	setCmdBuf(buffer, length);

	/*
	 *  Initialize the token module.
	 */
	if (strIsExpr) {
	    inputStr = str;
	    tokenInit(TRUE);
	} else {
	    if (STREQ(str, "-")) {
		inputFile = stdin;
	    } else {
		inputFile = fopen(str, "r");
		if (inputFile == NULL) {
		    error("cannot open the expression file: %s", str);
		}
	    }
	    tokenInit(FALSE);
	}

	/*
	 *  Read the first token.
	 */
	if (! getToken())
	    return -1;

	/*
	 *  Parse the expression.
	 */
	switch (expression()) {
	    case PARSE_OK:
		constantExpr = oneNumCmd();
		return numCmds();
	    case PARSE_NOTHING:
		if (tokenType == TOKEN_EOI)
		    return 0;
		usrerr("Expected operand or '('");
		return -1;
	    case PARSE_ERROR:
		return -1;
	}
}
