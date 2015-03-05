/*
 * loop thru sequence
 */

#include <errno.h>
#include <stdio.h>
#include "ipw.h"

void
seqloop(
	REG_1 double	start,			/* start of range	*/
	REG_2 double	fin,			/* end of range		*/
	REG_3 double	inc,			/* increment		*/
	double		(*inv)(double x),	/* -> inverse function	*/
	const char     *fmt)			/* output format	*/
{
	if (fin > start) {

		if (inc <= 0) {
			error("increment negative or zero");
		}

		/*
		 * print out values
		 */

		if (inv != NULL) {
			errno = 0;
			while (start <= fin) {
				(void)printf(fmt, (*inv)(start));
				if (errno) {
					error("math function error");
				}
				start += inc;
				putchar('\n');
			}
		}

		else {
			while (start <= fin) {
				(void)printf(fmt, start);
				start += inc;
				putchar('\n');
			}
		}
	}

	else {
		if (inc >= 0) {
			error("increment positive or zero");
		}

		/*
		 * print out values
		 */

		if (inv != NULL) {
			errno = 0;
			while (start >= fin) {
				(void)printf(fmt, (*inv)(start));
				if (errno) {
					error("math function error");
				}
				start += inc;
				putchar('\n');
			}
		}
		else {
			while (start >= fin) {
				(void)printf(fmt, start);
				start += inc;
				putchar('\n');
			}
		}
	}
}
