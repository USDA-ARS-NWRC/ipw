/* refice - refractive index of ice */

#include <math.h>

#include "ipw.h"
#include "pgm.h"

#include "icedata.h"

#define genref		refice	/* routine w/o temp dependence		*/
#define genreft		trefice	/* routine w temp dependence		*/

#define FUNC	"refice"
#define FUNCT	"trefice"

#include "genref.cpp"
#include "genreft.cpp"
