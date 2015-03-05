#include "ipw.h"
#include "pgm.h"
#include <stdio.h>
#include <string.h>

extern char *strtok();

void
pnm_header(void)
{
	char line[MAXLINE];

/*
 * get type
 */

	if(ugets(parm.i_fd,line,MAXLINE)==NULL)
		error("can't open input file");

	if ( (strncmp(line,PPMCOOKIE,strlen(PPMCOOKIE)))==0)
		parm.nbands=3;
	else if ( (strncmp(line,PGMCOOKIE,strlen(PGMCOOKIE)))==0)
		parm.nbands=1;
	else
		error("unknown header:\n\t\t%s",line);

/*
 * skip comments
 */

loop:
	if( (ugets(parm.i_fd,line,MAXLINE)) == NULL)
		error("input has a PNM header but no pixel data");
	if (line[0] == '#') goto loop;

/*
 * read size line
 */

	parm.nsamps=atoi(strtok(line," "));
	parm.nlines=atoi(strtok(NULL," "));

/*
 * skip max pixel value line
 */

	if( (ugets(parm.i_fd,line,MAXLINE)) == NULL)
		error("could not skip past last PNM header");

}
