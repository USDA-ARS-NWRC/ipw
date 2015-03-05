#include "ipw.h"
#include "bih.h"
#include "pgm.h"

/*
 * dm_val -- calculate dither matrix value
 *
 *	Algorithm derived from recurrence relation given on p. 107 of:
 *		David F. Rogers
 *		"Procedural Elements for Computer Graphics."
 *		McGraw-Hill
 *		1985
 */

int
dm_val(
	int	rank,		/* dither matrix rank		 */
	int	row,		/* matrix row index		 */
	int	col)		/* matrix column index		 */
{
	static int      dm2x2[2][2] = {	/* 2 x 2 dither matrix		 */
		{0, 2},
		{3, 1}
	};

	if (rank > 2) {
		return (4 * dm_val(rank / 2, row, col)
			+ dm_val(2, (2 * row) / rank, (2 * col) / rank));
	}

	return (dm2x2[row & 1][col & 1]);
}
