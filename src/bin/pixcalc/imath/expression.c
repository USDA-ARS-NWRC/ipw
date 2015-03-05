/*
** NAME
**      expression -- parse an expression
**
** SYNOPSIS
**	int
**	expression(void)
**
** DESCRIPTION
**	This routine is a recursive-descent parser that handles an
**	expression.  The parser recognizes various levels of precedence
**	in expressions by defining an expression at a certain level in
**	terms of the next higher-level.  The highest level is level 1, so
**	a level-2 expression is defined in terms of level-1 expressions.
**	Likewise, a level-3 expression is defined in terms of level-2
**	expressions.  And so on.  In general, the grammar for level N
**	looks like this:
**
**	    level_N expression ::= unary-op_N operand_N
**				 | operand_N [ binary-op_N operand_N ] ...
**
**	where	unary-op_N = unary operators for level N
**		binary-op_N = binary operators for level N
**		operand_N = operand for level N (usually level N-1 expression)
**		
**	The grammar for a complete expression {expression} is:
**
**	    level 1 expression (highest precedence: unary minus)
**
**		unary-op_1  ::= "-"
**		binary-op_1 ::= {nothing}
**		operand_1   ::= number | band-reference | special-variable
**			      | function-call | "(" expression ")"
**
**		number ::= integer | floating-point-#
**		band-reference ::= band-identifier "[" expression "]"
**				 | band-identifier {concatenated with} unsigned
**				   integer
**		band-identifier ::= "band" | "b"
**		special-variable ::= {one of the pre-defined variables}
**		function-call ::= function-name "(" expression
**						    ["," expression] ... ")"
**		function-name ::= {one of the pre-defined function names}
**
**	      so, expr_1 ::= [ "-" ] operand_1
**
**
**	    level 2 expression (exponentation)
**
**		unary-op_2  ::= {nothing}
**		binary-op_2 ::= "^"
**		operand_2   ::= expr_1
**
**	      so, expr_2 ::= expr_1 [ "^" expr_1 ] ...
**
**
**	    level 3 expression (multiplication, division)
**
**		unary-op_3  ::= {nothing}
**		binary-op_3 ::= "*" | "/"
**		operand_3   ::= expr_2
**
**	      so, expr_3 ::= expr_2 [ ( "*" | "/" ) expr_2 ] ...
**
**
**	    level 4 expression (addition, subtraction)
**
**		unary-op_4  ::= {nothing}
**		binary-op_4 ::= "+" | "-"
**		operand_4   ::= expr_3
**
**	      so, expr_4 ::= expr_3 [ ( "+" | "-" ) expr_3 ] ...
**
**
**	    level 5 expression (relational operators)
**
**		unary-op_5  ::= {nothing}
**		binary-op_5 ::= "=" | "<>" | "!=" | "<" | "<=" | ">" | ">="
**			      | "=>" | "=<"
**		operand_5   ::= expr_4
**
**	      so, expr_5 ::= expr_4 [ relation_op  expr_4 ]
**
**	    level 6 expression (unary not)
**
**		unary-op_6  ::= "not" | "!"
**		binary-op_6 ::= {nothing}
**		operand_6   ::= expr_5
**
**	      so, expr_6 ::= [ "not" | "!" ] expr_5
**
**	    level 7 expression (logical AND and OR)
**
**		unary-op_7  ::= {nothing}
**		binary-op_7 ::= "and" | "&&" | "or" | "||"
**		operand_7   ::= expr_6
**
**	      so, expr_7 ::= expr_6 [ ( "and" | "&&" | "or" | "||") expr_6 ] ...
**
**	    level 8 expression (trinary conditional operator)
**
**	      expr_8 ::= expr_7 [ "?" expr_8 ":" expr_8 ]
**
**	So a complete expression refers to this last level:
**
**	     expression ::= expr_8
**
**	This parser requires that the current token has already been read,
**	and its type is in the global variable "tokenType".
**
** RETURN VALUE
**
**	PARSE_OK	An expression was parsed successfully.
**
**	PARSE_NOTHING	Nothing was parsed, i.e., no tokens were consumed.
**
**	PARSE_ERROR	An error occured, and a message explaining the error
**			has been stored with the 'usrerr' routine.
**
** GLOBAL VARIABLES READ 
**	tokenFloat
**	tokenInteger
**	tokenPosition
**	tokenText
**	tokenType
**
** GLOBAL VARIABLES MODIFIED
*/

#include	"ipw.h"
#include        "pgm.h"
#include        "compile.h"
#include        "token.h"
#include	"cmdbuf.h"

/* ------------------------------------------------------------------------ */

/*
 *  band-reference ::= band-identifier "[" expression "]"
 *		     | band-identifier {concatenated with} unsigned integer
 *  band-identifer ::= "band" | "b"
 */

int
band_ref(void)
	{
	if (tokenType == TOKEN_BAND_AND_NUM)
	    {
	    putCmdAndInt(CMD_BAND_AND_NUM, tokenInteger);
	    if (! getToken())
		return PARSE_ERROR;
	    return PARSE_OK;
	    }

	if (tokenType != TOKEN_BAND)
	    return PARSE_NOTHING;
	if (! getToken())
	    return PARSE_ERROR;

	if (tokenType != TOKEN_L_BRACKET)
	    {
	    usrerr("Expected '['");
	    return PARSE_ERROR;
	    }
	if (! getToken())
	    return PARSE_ERROR;

	switch (expression())
	    {
	    case PARSE_NOTHING:
		usrerr("Expected expression");
		/* Fall through */
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	if (tokenType != TOKEN_R_BRACKET)
	    {
	    usrerr("Expected ']'");
	    return PARSE_ERROR;
	    }
	if (! getToken())
	    return PARSE_ERROR;

	putCmd(CMD_BAND);

	return PARSE_OK;
	}
/* ------------------------------------------------------------------------ */

/*
 *  special-variable ::= {one of the pre-defined variables}
 */

int
special_var(void)
{
	switch (tokenType)
	    {
	    case TOKEN_PI:
		putCmd(CMD_PI);
		break;
	    case TOKEN_E:
		putCmd(CMD_E);
		break;
	    case TOKEN_NBANDS:
		putCmd(CMD_NBANDS);
		break;
	    case TOKEN_SUM:
		putCmd(CMD_SUM);
		break;
	    default:
		return PARSE_NOTHING;
	    }
	if (! getToken())
	    return PARSE_ERROR;

	return PARSE_OK;
}

/* ------------------------------------------------------------------------ */

/*
 *  function-call ::= function-name "(" expression ["," expression] ... ")"
 *
 *  where
 *	function-name ::= {one of the pre-defined function names}
 */

int
function_call(void)
{
	int command;
	int min_args = 1;	/* default is 1 argument */
	int max_args = 1;
	int arg_count;
	int more_args;

	switch (tokenType)
	    {
	    case TOKEN_LOG:
		command = CMD_LOG;
		break;
	    case TOKEN_LOG10:
		command = CMD_LOG10;
		break;
	    case TOKEN_COS:
		command = CMD_COS;
		break;
	    case TOKEN_SIN:
		command = CMD_SIN;
		break;
	    case TOKEN_TAN:
		command = CMD_TAN;
		break;
	    case TOKEN_ARCCOS:
		command = CMD_ARCCOS;
		break;
	    case TOKEN_ARCSIN:
		command = CMD_ARCSIN;
		break;
	    case TOKEN_ARCTAN:
		command = CMD_ARCTAN;
		break;
	    case TOKEN_ROUND:
		command = CMD_ROUND;
		break;
	    case TOKEN_TRUNC:
		command = CMD_TRUNC;
		break;

	    case TOKEN_MIN:
		command = CMD_MIN;
		min_args = 0;		/* any number of arguments */
		max_args = -1;
		break;
	    case TOKEN_MAX:
		command = CMD_MAX;
		min_args = 0;		/* any number of arguments */
		max_args = -1;
		break;
	    case TOKEN_MEDIAN:
		command = CMD_MEDIAN;
		min_args = 0;		/* any number of arguments */
		max_args = -1;
		break;

	    default:
		return PARSE_NOTHING;
	    }
	if (! getToken())
	    return PARSE_ERROR;

	/*
	 *  Okay, now look for the required # of arguments
	 */
	if (tokenType != TOKEN_L_PAREN)
	    {
	    /*
	     *  If function requires no arguments (i.e., it can be used as
	     *	a special variable), then put its command with 0 args.
	     */
	    if (min_args == 0)
		{
		putCmdAndInt(command, 0);
		return PARSE_OK;
		}
	    else
		{
		usrerr("Expected '('");
		return PARSE_ERROR;
		}
	    }
	if (! getToken())
	    return PARSE_ERROR;

	for (arg_count = 0, more_args = TRUE;
		((max_args < 0) || (arg_count < max_args)) && (more_args);
		arg_count++)
	    {
	    if (arg_count > 0)
		{
		if (tokenType != TOKEN_COMMA)
		    {
		    if (max_args < 0)
			{
			more_args = FALSE;
			arg_count--;
			continue;
			}
		    else
			{
			usrerr("Expected ','");
			return PARSE_ERROR;
			}
		    }
		if (! getToken())
		    return PARSE_ERROR;
		}

	    switch (expression())
		{
		case PARSE_NOTHING:
		    if (arg_count < min_args)
			{
			usrerr("Expected expression");
			return PARSE_ERROR;
			}
		    else
			{
			more_args = FALSE;
			continue;
			}
		case PARSE_ERROR:
		    return PARSE_ERROR;
		}
	    } /* for */

	if (tokenType != TOKEN_R_PAREN)
	    {
	    usrerr("Expected ')'");
	    return PARSE_ERROR;
	    }
	if (! getToken())
	    return PARSE_ERROR;

	/*
	 *  If the minimum and maximum are the different, put the # of
	 *  arguments.
	 */
	if (min_args != max_args) {
	    putCmdAndInt(command, arg_count);
	} else {
	    putCmd(command);
	}

	return PARSE_OK;
}

/* ------------------------------------------------------------------------ */

/*
 *  Level 1 expression ::= [ "-" ] operand_1
 *
 *  where
 *  	operand_1 ::= number | band-reference | special-variable
 *		    | function-call | | "(" expression ")" 
 *	number ::= integer | floating-point-#
 */

int
expr_1(void)
	{
	int unaryMinus;

	if (tokenType == TOKEN_MINUS)
	    {
	    unaryMinus = TRUE;
	    if (! getToken())
		return PARSE_ERROR;
	    }
	else
	    unaryMinus = FALSE;

	if (tokenType == TOKEN_INTEGER)
	    {
	    putCmdAndInt(CMD_INTEGER, tokenInteger);
	    if (! getToken())
		return PARSE_ERROR;
	    goto checkMinus;
	    }

	if (tokenType == TOKEN_FLOAT)
	    {
	    putCmdAndFloat(CMD_FLOAT, tokenFloat);
	    if (! getToken())
		return PARSE_ERROR;
	    goto checkMinus;
	    }

	/*
	 *  Parenthesized expression
	 */
	if (tokenType == TOKEN_L_PAREN)
	    {
	    if (! getToken())
		return PARSE_ERROR;

	    switch (expression())
		{
		case PARSE_NOTHING:
		    usrerr("Expected expression");
		    /* Fall through */
		case PARSE_ERROR:
		    return PARSE_ERROR;
		}

	    if (tokenType != TOKEN_R_PAREN)
		{
		usrerr("Expected ')'");
		return PARSE_ERROR;
		}
	    if (! getToken())
		return PARSE_ERROR;

	    return PARSE_OK;
	    }  /* if '(' */


	switch (band_ref())
	    {
	    case PARSE_OK:
		goto checkMinus;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	switch (special_var())
	    {
	    case PARSE_OK:
		goto checkMinus;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	switch (function_call())
	    {
	    case PARSE_OK:
		goto checkMinus;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	if (unaryMinus)
	    {
	    usrerr("Expected a basic expression operand");
	    return PARSE_ERROR;
	    }
	else
	    return PARSE_NOTHING;

	/* ---------------------------------------------------------------- */

    checkMinus:
	if (unaryMinus)
	    putCmd(CMD_NEGATE);
	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 2 expression ::= expr_1 [ "^" expr_1 ] ...
 *
 *  NOTE:  Exponentation is right-associative, e.g.,
 *	   x ^ y ^ z = x ^ (y ^ z)
 */

int
expr_2(void)
	{
	int op_count = 0;	/* # of "^" operators found */

	switch (expr_1())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	while (tokenType == TOKEN_CARET)
	    {
	    op_count++;
	    if (! getToken())
		return PARSE_ERROR;

	    switch (expr_1())
		{
		case PARSE_NOTHING:
		    usrerr("Expected '-' or a basic expression operand");
		    /* Fall through */
		case PARSE_ERROR:
		    return PARSE_ERROR;
		}
	    }  /* while */

	for (; op_count > 0; op_count--)
	    putCmd(CMD_EXPONENTATION);

	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 3 expression ::= expr_2 [ ( "*" | "/" ) expr_2 ] ...
 */

int
expr_3(void)
	{
	int command;

	switch (expr_2())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	while ((tokenType == TOKEN_STAR) || (tokenType == TOKEN_SLASH))
	    {
	    switch (tokenType)
		{
		case TOKEN_STAR:
		    command = CMD_MULTIPLY;
		    break;
		case TOKEN_SLASH:
		    command = CMD_DIVIDE;
		    break;
		}
	    if (! getToken())
		return PARSE_ERROR;

	    switch (expr_2())
		{
		case PARSE_NOTHING:
		    usrerr("Expected '-' or a basic expression operand");
		    /* Fall through */
		case PARSE_ERROR:
		    return PARSE_ERROR;
		}

	    putCmd(command);
	    }  /* while */

	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 4 expression ::= expr_3 [ ( "+" | "-" ) expr_3 ] ...
 */

int
expr_4(void)
	{
	int looking4ops = TRUE;
	int command;

	switch (expr_3())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	/*
	 *  We're looking for one of the level-4 binary operators (+, -).
	 *  We have to watch out for the special case where the operand
	 *  following the "-" operator is a positive number without any
	 *  intervening whitespace, e.g.,
	 *
	 *	band[0]-5 * 6
	 *
	 *  The token function will treat the "-5" as a single token
	 *  (i.e., a negative integer).  But we want to treat it as two
	 *  tokens: "-" followed by the integer "5".
	 *
	 *  So when we're looking for operators, and we have a negative
	 *  number, we treat the negative number as the "-" operator
	 *  followed by the *positive* equivalent of the number.
	 */
	while (looking4ops)
	    {
	    if (tokenType == TOKEN_PLUS)
		{
		command = CMD_ADD;
		if (! getToken())
		    return PARSE_ERROR;
		}
	    else if (tokenType == TOKEN_MINUS)
		{
		command = CMD_SUBTRACT;
		if (! getToken())
		    return PARSE_ERROR;
		}
	   else if ((tokenType == TOKEN_INTEGER) && (tokenInteger < 0))
		{
		command = CMD_SUBTRACT;
		tokenInteger = -tokenInteger;
		}

	   else if ((tokenType == TOKEN_FLOAT) && (tokenFloat < 0))
		{
		command = CMD_SUBTRACT;
		tokenFloat = -tokenFloat;
		}

	   else {
		looking4ops = FALSE;
		continue;  /* skip to top of "while" loop */
	        }

	   /*
	    *  Look for the following level-3 expression.
	    */
	   switch (expr_3())
		{
		case PARSE_NOTHING:
			usrerr("Expected '-' or a basic expression operand");
			/* Fall through */
		case PARSE_ERROR:
			return PARSE_ERROR;
		}

	   putCmd(command);
	   }  /* while */

	return PARSE_OK;
}

/* ------------------------------------------------------------------------ */

/*
 *  Level 5 expression ::= expr_4 relation-op expr_4
 *
 *  where
 *	relation-op := "=" | "!=" | "<>" | "<" | "<=" | "=<" | ">" | ">="
 */

int
expr_5(void)
	{
	int command;

	switch (expr_4())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	switch (tokenType)
	    {
	    case TOKEN_EQUAL:
		command = CMD_EQUAL;
		break;
	    case TOKEN_NOT_EQUAL:
		command = CMD_NOT_EQUAL;
		break;
	    case TOKEN_LESS:
		command = CMD_LESS;
		break;
	    case TOKEN_LESS_EQUAL:
		command = CMD_LESS_EQUAL;
		break;
	    case TOKEN_GREATER:
		command = CMD_GREATER;
		break;
	    case TOKEN_GREATER_EQUAL:
		command = CMD_GREATER_EQUAL;
		break;
	    default:
		return PARSE_OK;
	    }
	if (! getToken())
		return PARSE_ERROR;

	switch (expr_4())
	    {
	    case PARSE_NOTHING:
		    usrerr("Expected expression");
		    /* Fall through */
	    case PARSE_ERROR:
		    return PARSE_ERROR;
	    }

	putCmd(command);
	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 6 expression ::= [ "not" | "!" ] expr_5
 */

int
expr_6(void)
	{
	int unaryNot;

	if (tokenType == TOKEN_NOT)
	    {
	    unaryNot = TRUE;
	    if (! getToken())
		return PARSE_ERROR;
	    }
	else
	    unaryNot = FALSE;

	switch (expr_5())
	    {
	    case PARSE_NOTHING:
		if (unaryNot)
		    {
		    usrerr("Expected expression");
		    return PARSE_ERROR;
		    }
		else
		    return PARSE_NOTHING;

	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	if (unaryNot)
	    putCmd(CMD_NOT);
	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 7 expression ::= expr_6 [ ( "and" | "&&" | "or" | "||" ) expr_6 ] ...
 */

int
expr_7(void)
	{
	int command;

	switch (expr_6())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	while ((tokenType == TOKEN_AND) || (tokenType == TOKEN_OR))
	    {
	    switch (tokenType)
		{
		case TOKEN_AND:
		    command = CMD_AND;
		    break;
		case TOKEN_OR:
		    command = CMD_OR;
		    break;
		}
	    if (! getToken())
		return PARSE_ERROR;

	    switch (expr_6())
		{
		case PARSE_NOTHING:
		    usrerr("Expected expression");
		    /* Fall through */
		case PARSE_ERROR:
		    return PARSE_ERROR;
		}

	    putCmd(command);
	    }  /* while */

	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Level 8 expression ::= expr_7 [ "?" expr_8 ":" expr_8 ]
 */

int
expr_8(void)
	{
	command_t * firstGoto;
	command_t * secondGoto;

	switch (expr_7())
	    {
	    case PARSE_NOTHING:
		return PARSE_NOTHING;
	    case PARSE_ERROR:
		return PARSE_ERROR;
	    }

	if (tokenType != TOKEN_QUESTION)
	    return PARSE_OK;

	if (! getToken())
		return PARSE_ERROR;

	/*
	 *  Put the command to go to the start of the FALSE expression.
	 *  Store its location so we can store offset later on.
	 */
	firstGoto = putCmd(CMD_GOTO_IF_0);

	switch (expr_8())
	    {
	    case PARSE_NOTHING:
		    usrerr("Expected expression");
		    /* Fall through */
	    case PARSE_ERROR:
		    return PARSE_ERROR;
	    }

	/*
	 *  Put the command to go to end of the FALSE expression.
	 */
	secondGoto = putCmd(CMD_GOTO);

	/*
	 *  Set the destination of the first GOTO = next open spot in buffer.
	 */
	setDest(firstGoto);

	/*
	 *  Fetch the FALSE expression.
	 */
	if (tokenType != TOKEN_COLON)
	    {
	    usrerr("Expected colon ':'");
	    return PARSE_ERROR;
	    }
	if (! getToken())
		return PARSE_ERROR;

	switch (expr_8())
	    {
	    case PARSE_NOTHING:
		    usrerr("Expected expression");
		    /* Fall through */
	    case PARSE_ERROR:
		    return PARSE_ERROR;
	    }

	/*
	 *  Set the destination of the second GOTO = next open spot in buffer.
	 */
	setDest(secondGoto);

	return PARSE_OK;
	}

/* ------------------------------------------------------------------------ */

/*
 *  Complete expression ::= expr_8
 */

int
expression(void)
	{
	return expr_8();
	}
