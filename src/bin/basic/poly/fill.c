
#include "ipw.h"

#include "pgm.h"

void
fill(
	REG_1 int       x,
	REG_2 int       y)
{
	if (buf[x][y] != MARK) {
		buf[x][y] = MARK;

		fill(x + 1, y);
		fill(x - 1, y);
		fill(x, y + 1);
		fill(x, y - 1);
	}
}
