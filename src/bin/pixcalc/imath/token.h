#ifndef TOKEN_H
#define TOKEN_H

/*
 *  The include file for tokens.
 */

/* ------------------------------------------------------------------------ */

#include "tokentypes.h"

/* ------------------------------------------------------------------------ */

/*
 *  Global variables with token information.
 */

extern	char * inputStr;		/* input string with expression	     */
extern	FILE * inputFile;		/* input file with expression	     */

extern	int    lineNum;			/* current line in input file	     */
extern	int    tokenType;		/* type of current token 	     */
extern	int    tokenPosition;		/* position of token on line (>= 0)  */
extern	int    tokenInteger;		/* value of integer token  	     */
extern	float  tokenFloat;		/* value of floating-point token     */
extern	char * tokenText;		/* token's text (for identifiers)    */
extern	char **bandNames;		/* array of valid band names	     */
extern	int    nameCount;		/* # of band names in array	     */

/* ------------------------------------------------------------------------ */

/*
 *  Routines for tokens.
 */

void	tokenInit(bool_t inputFromStr);
bool_t	validIdentifier(char *str);
int	getToken(void);

#ifdef DEBUG
char *	typeAsString(int type);
#endif

#endif  /* TOKEN_H */
