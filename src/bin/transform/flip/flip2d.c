/*
** Flip lines of image:
** Read entire image into 2-d memory.
** If necessary, flip samples within each line.
** Then flip dope vector that references beginning of each line.
*/

#include "ipw.h"
#include "pgm.h"

void
flip2d(
	int             nl,		/* # lines in image		 */
	int             ns,		/* # samples per line		 */
	int             nb,		/* # bytes per sample		 */
	addr_t         *buf)		/* data buffer			 */
{
	int             tbytes;		/* # bytes per image		 */
	int             linebytes;	/* # bytes per line		 */
	int             ngot;		/* # bytes read/written		 */
	int             j;		/* line counter			 */
	long            hold;


	hold = ns;
	hold *= nb;
	linebytes = ltoi(hold);

	hold *= nl;
	tbytes = ltoi(hold);

 /*
  * read the entire image into the buffer
  */

	ngot = uread(parm.i_fd, buf[0], tbytes);
	if (ngot != tbytes) {
		if (ngot < 0)
			error("image read error");
		else
			error("unexpected EOF on input image");
	}

	if (parm.samps) {

 /*
  * flip order of samples within lines
  */

		j = nl;
		while (--j >= 0) {
			reverse(buf[j], ns, nb);
		}
	}

 /*
  * flip order of lines
  */

 /* NOSTRICT */
	reverse((addr_t) buf, nl, sizeof(addr_t));


 /*
  * write the image data
  */

	for (j = 0; j < nl; ++j) {
		ngot = uwrite(parm.o_fd, buf[j], linebytes);
		if (ngot != linebytes)
			error("image write error");
	}
}
