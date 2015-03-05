#include "IPWmacros.h"

extern void
	bresenham(int x1, int y1, int x2, int y2);

extern void
	fill(REG_1 int x, REG_2 int y);

extern void
	poly(FILE *fpi, bool_t sflag, int xseed, int yseed);

extern char	**buf;

#define	MARK		1
