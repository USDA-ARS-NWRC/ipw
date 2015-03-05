/*
** NAME
**	getf -- get a floating-pt. number, aggressively
**
** SYNOPSIS
**	#include "ipw.h"
**
**	double
**	getf(
**		char	*prompt,	|* string to prompt user	*|
**		double	minval,		|* minimum acceptable value	*|
**		double	maxval)		|* maximum acceptable value	*|
**
** DESCRIPTION
**	getf writes prompt on the standard output, followed by a ?
**	The user must type a real number in the range minval <-> maxval,
**	or getf will prompt again.  If maxval < minval, the number
**	is only required to be >= minval.  The number is returned.
**
** HISTORY
**	spring 1981: 	written by J.Frew, CSL, UCSB
**	May 1995	Converted from QDIPS to IPW by J. Domingo, OSU
*/

#include	<unistd.h>

#include	"ipw.h"

double
getf(
	char	*prompt,	/* string to prompt user	*/
	double	minval,		/* minimum acceptable value	*/
	double	maxval)		/* maximum acceptable value	*/
{
	double	val;

	while (TRUE) {
		if (isatty(STDIN_FILENO)) {
			printf("%s? ", prompt);
			fflush(stdout);
		}
		if (scanf("%lf", &val) == EOF)
			ipwexit(EXIT_SUCCESS);

		if (maxval < minval) {	/*  just check lower bound  */
			if (val >= minval)
				break;

			printf("(%g) must be >= %g\n", val, minval);
			fflush(stdout);
		}
		else {
			if (val >= minval  &&  val <= maxval)
				break;

			printf("(%g) must be >= %g and <= %g\n",
				val, minval, maxval);
			fflush(stdout);
		}
	}

	return (val);
}
