/*
 *  Module for tokens.
 */

#include <stdlib.h>	/* for strtol() and strtod() */
#include <ctype.h>
#include <string.h>

#include "ipw.h"
#include "pgm.h"
#include "token.h"

/*
 *  The size of the input buffer is 1 more than the maximum input-line length.
 *  One extra character for the null character ('\0').
 */
#define BUFFER_SIZE (LINE_MAX + 1)

/*
 *  Comments start with this character and extend to the end of the current
 *  line.
 */
#define COMMENT_CHAR '#'

/* ------------------------------------------------------------------------ */

/*
 *  Public global variables.
 */

char * inputStr = NULL;		/* input string with expression	     */
FILE * inputFile = NULL;	/* input file with expression	     */

int    lineNum = 0;		/* current line in input file	     */
int    tokenType = -1;		/* type of current token 	     */
int    tokenPosition;		/* position of token on line (>= 0)  */
int    tokenInteger;		/* value of integer token  	     */
float  tokenFloat;		/* value of floating-point token     */
char * tokenText;		/* token's text (for identifiers)    */
char **bandNames = NULL;	/* array of valid band names	     */
int    nameCount = 0;		/* # of band names in array	     */

/* ------------------------------------------------------------------------ */

/*
 *  Private global variables.
 */

static char buffer[BUFFER_SIZE] = { '\0' };
    /*
     *  Holds the string being separated into tokens.
     */

static char * ptr = buffer;
    /*
     *	Pointer to current spot in the buffer.
     */

static bool_t endOfInput = FALSE;
    /*
     *	Reached end of input string?
     */

static bool_t charReplaced = FALSE;
    /*
     *  TRUE if the character pointed to by 'ptr' was replaced with
     *	a NULL character during the last call to the "getToken" function.
     */

static char savedChar;
    /*
     *  Holds the character pointed to by 'ptr' that was replaced with
     *	a NULL character during the last call to the "getToken" function.
     */

/* ------------------------------------------------------------------------ */

/*
** NAME
**      tokenInit -- initialize the token module
**
** SYNOPSIS
**	#include "token.h"
**
**	void
**	tokenInit(
**		bool_t inputFromStr)	|* is input from string? (or file?) *|
**
** DESCRIPTION
**	This routine initializes the token module.  A token is a word or
**	symbol in the input, e.g., "band" or "*".  Tokens are read
**	from either a given input string in the global variable "inputStr"
**	or from an input file (global variable "inputFile").
**
** GLOBAL VARIABLES READ 
**	inputStr
**	inputFile
**
** GLOBAL VARIABLES MODIFIED
*/

void
tokenInit(
	bool_t inputFromStr)	/* is input from string? (or file?) */
{
	if (inputFromStr) {
	    assert(inputStr != NULL);
	    strcpy(buffer, inputStr);

	} else {
	    assert(inputFile != NULL);
	    if (fgets(buffer, BUFFER_SIZE, inputFile) == NULL) {
		if (feof(inputFile)) {
		    endOfInput = TRUE;
		} else {
		    error("Cannot read first line from file with expression");
		}
	    } else {
		lineNum++;
	    }
	}
}

/* ------------------------------------------------------------------------ */

/*
** NAME
**      validIdentifier -- is a string a valid identifier?
**
** SYNOPSIS
**	#include "token.h"
**
**	bool_t
**	validIdentifier(
**		char * str)	|* input string to check *|
**
** DESCRIPTION
**	This routine checks if the given string is a valid identifier:
**
**	    identifier = letter followed by 1 or more letters, digits, and
**	    		 underscores
**
** RETURN VALUE
**	TRUE	The given string is a valid identifier
**
**	FALSE	The string isn't a valid identifier
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
**	tokenText
*/

bool_t
validIdentifier(
	char * str)	/* input string to check */
{
	assert(str != NULL);

	if (! isalpha(*str))
	    return FALSE;

	while (isalnum(*str) || (*str == '_'))
	    str++;

	return *str == '\0';
}

/* ------------------------------------------------------------------------ */

/*
** NAME
**      getToken -- get next token from input string or file
**
** SYNOPSIS
**	#include "token.h"
**
**	int
**	getToken(void)
**
** DESCRIPTION
**	This routine fetches the next token from the input string or file.
**	The routine "tokenInit" must be called before the first call
**	to this routine.
**
**	This routine returns the token's type in the variable 'token'.
**	Some token types have additional information returned by other
**	variables:
**
**		TOKEN_INTEGER		tokenInteger = token's integer value
**		TOKEN_FLOAT		tokenFloat = token's float value
**		TOKEN_IDENTIFIER	tokenText = identifier's name
**		TOKEN_BAND_AND_NUM	tokenInteger = band number in token
**
**	The position of the starting character of the token is stored in
**	the variable 'tokenPosition'.  If the tokens are being read from
**	an input file, the current line number is in the variable 'lineNum'.
**
** RETURN VALUE
**
**	TRUE	A token was successfully read in.
**
**	FALSE	An error occured, and a message explaining the error has
**		been stored with the "usrerr" routine.
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
**	lineNum
**	tokenFloat
**	tokenInteger
**	tokenPostion
**	tokenText
*/

/* ------------------------------------------------------------------------ */

/*
 *  Table of reserved words.
 */
static struct {
	char * text;
	int type;
} reservedWords[] = {
			{ "band",	TOKEN_BAND	},
			{ "b",		TOKEN_BAND	},
			{ "not",	TOKEN_NOT	},
			{ "and",	TOKEN_AND	},
			{ "or",		TOKEN_OR	},
			{ "pi",		TOKEN_PI	},
			{ "e",		TOKEN_E		},
			{ "nbands",	TOKEN_NBANDS	},
			{ "sum",	TOKEN_SUM	},
			{ "log",	TOKEN_LOG	},
			{ "log10",	TOKEN_LOG10	},
			{ "cos",	TOKEN_COS	},
			{ "sin",	TOKEN_SIN	},
			{ "tan",	TOKEN_TAN	},
			{ "arccos",	TOKEN_ARCCOS	},
			{ "arcsin",	TOKEN_ARCSIN	},
			{ "arctan",	TOKEN_ARCTAN	},
			{ "trunc",	TOKEN_TRUNC	},
			{ "round",	TOKEN_ROUND	},
			{ "min",	TOKEN_MIN	},
			{ "max",	TOKEN_MAX	},
			{ "median",	TOKEN_MEDIAN	},
			{ NULL,		-1		}
		     };

/* ------------------------------------------------------------------------ */

int
getToken(void)
{
	int i;


	if (endOfInput)
	    {
	    tokenType = TOKEN_EOI;
	    return TRUE;
	    }

	if (charReplaced)
	    *ptr = savedChar;
	charReplaced = FALSE;

	/*
	 *  Skip whitespace
	 */
  Skip:	while (isspace(*ptr))
	    ptr++;

	/*
	 *  Do we have a comment?  If so, skip rest of line by truncating
	 *  the string in the buffer.
	 */
	if (*ptr == COMMENT_CHAR)
	    *ptr = '\0';

	/*
	 *  Are we at the end of the current line/string?
	 */
	if (*ptr == '\0') {
	    /*
	     *  If we're reading from a string, we're done.
	     */
	    if (inputFile == NULL) {
		endOfInput = TRUE;
		tokenType = TOKEN_EOI;
		return TRUE;
	    }

	    /*
	     *  Otherwise, we're reading from a file, so get the next line,
	     *	and then go back to skipping whitespace.
	     */
	    else {
		lineNum++;
		if (fgets(buffer, BUFFER_SIZE, inputFile) == NULL) {
		    if (feof(inputFile)) {
			endOfInput = TRUE;
			tokenType = TOKEN_EOI;
			return TRUE;
		    }
		    error("Cannot read line %d from the expression file",
			  lineNum);
		    }
		ptr = buffer;
		goto Skip;
	    }
	}

	/*
	 *  Check for single- and double-character tokens (e.g., operators,
	 *  punctuation)
	 */
	tokenText = ptr++;
	tokenPosition = tokenText - buffer;

	switch (*tokenText)
	    {
	    case '+' :
		tokenType = TOKEN_PLUS;
		return TRUE;
	    case '/' :
		tokenType = TOKEN_SLASH;
		return TRUE;
	    case '*' :
		tokenType = TOKEN_STAR;
		return TRUE;
	    case '^' :
		tokenType = TOKEN_CARET;
		return TRUE;

	    case ',' :
		tokenType = TOKEN_COMMA;
		return TRUE;
	    case '(' :
		tokenType = TOKEN_L_PAREN;
		return TRUE;
	    case ')' :
		tokenType = TOKEN_R_PAREN;
		return TRUE;
	    case '[' :
		tokenType = TOKEN_L_BRACKET;
		return TRUE;
	    case ']' :
		tokenType = TOKEN_R_BRACKET;
		return TRUE;
	    case '{' :
		tokenType = TOKEN_L_BRACE;
		return TRUE;
	    case '}' :
		tokenType = TOKEN_R_BRACE;
		return TRUE;

	    case '?' :
		tokenType = TOKEN_QUESTION;
		return TRUE;
	    case ':' :
		tokenType = TOKEN_COLON;
		return TRUE;

	    case '=' :
		if (*ptr == '>')
		    {
		    ptr++;
		    tokenType = TOKEN_GREATER_EQUAL;
		    }
		else if (*ptr == '<')
		    {
		    ptr++;
		    tokenType = TOKEN_LESS_EQUAL;
		    }
		else
		    tokenType = TOKEN_EQUAL;
		return TRUE;
	    case '<' :
		if (*ptr == '=')
		    {
		    ptr++;
		    tokenType = TOKEN_LESS_EQUAL;
		    }
		else if (*ptr == '>')
		    {
		    ptr++;
		    tokenType = TOKEN_NOT_EQUAL;
		    }
		else
		    tokenType = TOKEN_LESS;
		return TRUE;
	    case '>' :
		if (*ptr == '=')
		    {
		    ptr++;
		    tokenType = TOKEN_GREATER_EQUAL;
		    }
		else
		    tokenType = TOKEN_GREATER;
		return TRUE;
	    case '!' :
		if (*ptr == '=')
		    {
		    ptr++;
		    tokenType = TOKEN_NOT_EQUAL;
		    }
		else
		    tokenType = TOKEN_NOT;
		return TRUE;

	    case '&' :
		if (*ptr == '&')
		    {
		    ptr++;
		    tokenType = TOKEN_AND;
		    return TRUE;
		    }
		break;

	    case '|' :
		if (*ptr == '|')
		    {
		    ptr++;
		    tokenType = TOKEN_OR;
		    return TRUE;
		    }
		break;

	    } /* switch */

	/*
	 *  Check for identifiers including band names and reserved words.
	 *  identifier = letter followed by 1 or more letters, digits, and
	 *  		 underscores
	 */
	if (isalpha(*tokenText))
	    {
	    while (isalnum(*ptr) || (*ptr == '_'))
		ptr++;

	    savedChar = *ptr;
	    *ptr = '\0';
	    charReplaced = TRUE;

	    for (i = 0; i < nameCount; i++) {
		if (strcmp(bandNames[i], tokenText) == 0)
		    {
		    tokenType = TOKEN_BAND_AND_NUM;
		    tokenInteger = i;
		    return TRUE;
		    }
	    }

	    for (i = 0; reservedWords[i].text != NULL; i++)
		if (strcmp(reservedWords[i].text, tokenText) == 0)
		    {
		    tokenType = reservedWords[i].type;
		    return TRUE;
		    }

	    tokenType = TOKEN_IDENTIFIER;

	    /*
	     *  Special case: "b" or "band" followed by an integer
	     *	For example, "b0" or "band3"
	     */
	    if (*tokenText == 'b')
		{
		char * start;
		char * p;

		start = tokenText + 1;
		if (strncmp(start, "and", 3) == 0)
		    start += 3;

		/*
		 *  Are remaining characters in the identifier digits?
		 */
		for (p = start; (*p != '\0') && isdigit(*p); p++)
		    ; /* empty loop body */
		if (*p == '\0')
		    {
		    tokenInteger = strtol(start, (char**) NULL, 10);
		    tokenType = TOKEN_BAND_AND_NUM;
		    }
		}

	    return TRUE;
	    }  /* 'if' identifier */

	/*
	 *  Check for unary minus sign ('-') or a negative number.
	 */
	if (*tokenText == '-') {
	    if (*ptr == '.')
		goto floatPt;
	    if (isdigit(*ptr))
		goto number;
	    tokenType = TOKEN_MINUS;
	    return TRUE;
	}

	/*
	 *  Check for a number: an integer or float-point number with an
	 *  integer part:
	 *
	 *		integer [ '.' integer ] [ exponent ]
	 *  where
	 *     		exponent ::= ( 'e' | 'E' ) [ '-' ] integer
	 */
	if (isdigit(*tokenText)) {

    number:
	    while (isdigit(*ptr))
		ptr++;

	    if (*ptr == '.')
		goto floatPt;
	    if (tolower(*ptr) == 'e')
		goto exponent;

	    tokenType = TOKEN_INTEGER;
	    tokenInteger = strtol(tokenText, (char**) NULL, 10);
	    return TRUE;
	}

	/*
	 *  Check for a float-point number with no leading integer part:
	 *
	 *		 '.' integer [ exponent ]
	 *  where
	 *     		exponent ::= ( 'e' | 'E' ) [ '-' ] integer
	 */
	if (*tokenText == '.') {
	    ptr = tokenText;

    floatPt:  /* ptr is pointing to '.' */
	    ptr++;
	    if (! isdigit(*ptr))
		{
		usrerr("Missing digit(s) after decimal point");
		return FALSE;
		}
	    do
		ptr++;
	    while (isdigit(*ptr));

	    if (tolower(*ptr) == 'e') {

    exponent:
		ptr++;
		if (*ptr == '-')
		    ptr++;
		if (! isdigit(*ptr))
		    {
		    usrerr("Missing exponent");
		    return FALSE;
		    }
		do
		    ptr++;
		while (isdigit(*ptr));
	    }

	    tokenType = TOKEN_FLOAT;
	    tokenFloat = strtod(tokenText, (char**) NULL);
	    return TRUE;
	}

	/*
	 *  We don't have a proper token.
	 */
	usrerr("Invalid word or symbol");
	return FALSE;
}

/* ------------------------------------------------------------------------ */

#if DEBUG

/*
 *  Array of token types and the highest token type.
 */
static char * tokenTypes[] = {
#include "tokentypes.c"
};

char *
typeAsString(
	int type)	/* token type to print */
{
	static char buffer[50];

	if ((type < 0) || (type >= sizeof(tokenTypes) / sizeof(char *)))
		{
		sprintf(buffer, "(unknown type = %d)", type);
		return buffer;
		}
	else
		return tokenTypes[type];
}

#endif
