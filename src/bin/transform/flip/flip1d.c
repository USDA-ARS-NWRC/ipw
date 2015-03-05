/*
** flip samples only, so can do line by line
*/

#include "ipw.h"
#include "pgm.h"

void
flip1d(
	int             nl,		/* # lines in image		 */
	int             ns,		/* # samples per line		 */
	int             nb,		/* # bytes per sample		 */
	addr_t          buf)		/* data buffer			 */

{
	int             linebytes;	/* # bytes per line		 */
	int             ngot;		/* # bytes read/written		 */
	long            hold;

	hold = ns;
	hold *= nb;
	linebytes = ltoi(hold);

	while (--nl >= 0) {

 /* read line */
		ngot = uread(parm.i_fd, buf, linebytes);
		if (ngot != linebytes) {
			if (ngot < 0)
				error("image read error");
			else
				error("unexpected EOF on input");
		}

 /* reverse line */
		reverse(buf, ns, nb);

 /* write line */
		ngot = uwrite(parm.o_fd, buf, linebytes);
		if (ngot != linebytes) {
			error("image write error");
		}
	}
}
