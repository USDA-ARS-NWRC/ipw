
#include "ipw.h"

#include "pgm.h"

#define	plot(x, y)	( buf[x][y] = MARK )

static int
sign(
	int             x)
{
	if (x > 0) {
		return (1);
	}

	if (x < 0) {
		return (-1);
	}

	return (0);
}

void
bresenham(
	int             x1,
	int             y1,
	int             x2,
	int             y2)
{
	REG_3 int       deltax;
	int             deltay;
	REG_2 int       err;
	REG_1 int       i;
	REG_6 int       interchange;
	int             s1;
	int             s2;
	int             tmp;
	REG_4 int       x;
	REG_5 int       y;

	x = x1;
	y = y1;
	deltax = abs(x2 - x1);
	deltay = abs(y2 - y1);
	s1 = sign(x2 - x1);
	s2 = sign(y2 - y1);

	/* interchange deltax,deltay depending on slope of line */

	if (deltay > deltax) {
		tmp = deltax;
		deltax = deltay;
		deltay = tmp;

		interchange = TRUE;
	}
	else {
		interchange = FALSE;
	}

	/* initialize error term to compensate for nonzero intercept */

	err = 2 * deltay - deltax;

	/* main loop */

	for (i = 1; i <= deltax; ++i) {
		plot(x, y);

		while (err >= 0) {
			if (interchange == 1) {
				x += s1;
			}
			else {
				y += s2;
			}

			err -= 2 * deltax;
		}

		if (interchange == 1) {
			y += s2;
		}
		else {
			x += s1;
		}

		err += 2 * deltay;
	}

	/* force last point to be plotted */

	plot(x2, y2);
}
