/*
** NAME
**	getok -- get decision from standard input
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	getok(
**		char	*prompt)
**
** DESCRIPTION
**	getok prompts on the standard output.  If the response begins
**	with 'y' or 'Y', it returns TRUE; otherwise, it returns FALSE.
**
**	If the standard input is not a terminal, no prompt is printed.
**
** HISTORY
**	spring 1981: 	written by J. Frew, CSL, UCSB
**	spring 1987: 	modified by D. Marks, SNARL, to correct bug
**			found under Sun Unix, V3.3
**	May 1995	Converted from QDIPS to IPW by J. Domingo, OSU
*/

#include	<unistd.h>

#include	"ipw.h"

int
getok(
	char	*prompt)
{
	char	answer[4];

	if (isatty(STDIN_FILENO)) {
		printf("%s (y|n)? ", prompt);
	}

	if (scanf("%s", answer) == EOF)
		ipwexit(EXIT_SUCCESS);

	if ((answer[0] == 'y') || (answer[0] == 'Y'))
		return(TRUE);
	else
		return(FALSE);
}
