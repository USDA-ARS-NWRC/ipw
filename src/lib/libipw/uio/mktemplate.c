/*
 * Copyright 2009, University of Idaho
 */

#include <string.h>

#include "ipw.h"

char *
mktemplate(
	const char     *prefix)		/* prefix for file name		 */
{
	char           *template;	/* template for temp file name	 */
        int		templateLen;
	const char     *tempdir = "/tmp";
	const char     *suffix = ".XXXXXX";
	
	assert(prefix != NULL);

	templateLen = strlen(tempdir) + 1 + strlen(prefix) + strlen(suffix) + 1;
	template = (char *) ecalloc(templateLen, sizeof(char));
	if (template == NULL) {
		return (NULL);
	}

	strcpy(template, tempdir);
	strcat(template, "/");
	strcat(template, prefix);
	strcat(template, suffix);

	return (template);
}
