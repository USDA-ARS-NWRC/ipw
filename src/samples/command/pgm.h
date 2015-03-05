/*
**  Sample header for an IPW program
**
**  The string "<NAME>" is a placeholder representing the name of the program
*/

#ifndef	PGM_H
#define	PGM_H

/*
 * header file for "<NAME>" program
 */

/*
**  First include any system headers needed for the typedefs, global variables,
**  and function prototypes in this file.
*/

#include <...>
...

/*
**  Next, always include the local header file with IPW macros
**/

#include "IPWmacros.h"


/*
**  Define "typedef"s.  Naming convention: all uppercase, ends with "_T"
*/

typedef struct {
	int	type;	  /* type of canopy cover 		*/
	float	height;	  /* average height of canopy cover (m) */
} CANOPY_T;

...

/*
**  Define any constants and macros.
*/

#define CONSTANT_1	43.6	/* Default coefficient */
...

#define MAX(x,y)	( (x > y) ? (x) : (y) )
/*
 *  Return the maximum of two numeric values.
 */

...


/*
**  Provide "extern" declarations for all global variables used within
**  the program.  Global variables are usually defined in the file "parm.c".
*/

extern int 	I_fd;	/* file descriptor for input image  */
extern int 	O_fd;	/* file descriptor for output image */
extern int 	M_fd;	/* file descriptor for mask image   */
...


/*
**  Function prototypes for all the local functions within the program.
*/

extern void     headers(void);
extern float    <NAME>(CANOPY_T canopy);
...


#endif  /* PGM_H */
