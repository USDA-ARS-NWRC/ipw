/*
** NAME
** 	get_classes -- read class definition file for lo, hi, rep triples
** 
** SYNOPSIS
**	get_classes (fp, nclass, classes)
**	FILE *fp;
**	int *nclass;
**	CLASS *classes;
** 
** DESCRIPTION
** 	get_classes reads the given input file for lo, hi, representative
**	value triples.
** 
*/

#include "ipw.h"
#include "crh.h"
#include "pgm.h"

static
int compar_classes(
	CLASS	*class1,
	CLASS	*class2);

void
get_classes (
	FILE		*fp,		/* file pointer to open class file  */
	int		*nclass,	/* number of classes                */
	CLASS	 	*classes)	/* class lo, hi, rep and class #'s  */
{
	int		nread;		/* number of data values read       */
	int		n;		/* current class #                  */
	int		i;		/* loop counter			    */

	n = 0;

	while ((nread = fscanf (fp, "%f %f %f", &classes[n].lo, &classes[n].hi,
				 &classes[n].rep)) > 0) {
		if (nread != 3) {
			error ("missing value for triple %d in class file\n",n);
		}
		classes[n].cls = n + 1;
		n++;
		if (n >= MAX_CLASS) {
			error ("too many classes");
		}
	}

	*nclass = n;

	if (*nclass < 2) {
		error ("must be at least two classes in input file");
	}

	/* sort in ascending order of ranges */

	SORT_ALG (classes, *nclass, sizeof(CLASS), compar_classes);

	/* check for sorting and continuity */

	for (i = 1; i < *nclass; i++) {
		if (classes[i].lo < classes[i-1].hi) {
			error ("classes overlapping or unsorted");
		} else if (classes[i].lo > classes[i-1].hi) {
			warn ("classes not continuous - undefined values will be set to NO_DATA (class 0)");
		}
	}

}

static
int compar_classes(
	CLASS	*cd1,
	CLASS	*cd2)
{
	if (cd1->rep > cd2->rep)
		return (1);
	else
		return (-1);
}
