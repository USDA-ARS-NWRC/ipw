/*
** NAME
** 	temp_filename -- create a temporary filename
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	temp_filename(
**	    char       *prefix,		|* prefix for temp file name	 *|
**	    char       *filename)	|* returned name of temp file	 *|
** 
** DESCRIPTION
**	temp_filename creates the name of a temporary file in the TMPDIR
**	directory.  The given prefix will be used to construct
**	the filename.  If an error occurs, the routine prints a message
**	and exits the program.
*/

#include <string.h>

#include "ipw.h"
#include "pgm.h"

void
temp_filename(
	char	       *prefix,		/* prefix for temp file name	 */
	char	       *filename)	/* returned name of temp file	 */
{
	char	       *tname;		/* temp name returned by tempnam */

	tname = mktemplate(prefix);
	if (tname == NULL) {
		error("Can't get name for temporary file");
	}

	int fdtemp = uwopen_temp(tname);
	if (fdtemp == ERROR) {
	    error("Can't create temporary file '%s'", tname);
	}
	uclose(fdtemp);

	strcpy(filename, tname);
}
