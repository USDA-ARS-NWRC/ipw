#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

static
void
int_output(
	char   *descrip,
	int	value)
{
	if ( parm.machread ) {
		printf("%d  ",value);
	}
	else {
		printf("%s: %d  ",descrip,value);
	}
}

/*
 * headers -- process image headers
 */

void
headers(void)
{
	BIH_T	**i_bihpp ;
	int	band ;

 /*
  * read BIH
  */

	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

 /*
  * output requested fields
  */


	if ( ISSET(DOFILE)  ) {
		if ( parm.machread ) {
			printf("\"%s\"  ",ufilename(parm.i_fd));	
		}
		else {
			printf("File: \"%s\"  ",ufilename(parm.i_fd));	
		}
	}

	if ( ISSET(DOBANDS) ) int_output("Bands",i_bihpp[0]->img->nbands);	
	if ( ISSET(DOLINES) ) int_output("Lines",i_bihpp[0]->img->nlines);	
	if ( ISSET(DOSAMPS) ) int_output("Samples",i_bihpp[0]->img->nsamps);	
	if ( ISSET(DOBYTES) ) {
		if ( ! parm.machread ) printf("Bytes/Pixel: ");
		for ( band = 0 ; band < i_bihpp[0]->img->nbands ; ++band ) 
			printf("%d ", i_bihpp[band]->nbytes);	
	
		}
	printf("\n");

 /*
  * done
  */
}
