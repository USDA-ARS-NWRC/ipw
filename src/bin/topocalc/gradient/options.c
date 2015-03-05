 /*
  * parse options
  */

#include "ipw.h"
#include "fpio.h"

void
options(
	OPTION_T       *opt_s,
	OPTION_T       *opt_a,
	OPTION_T       *opt_d,
	OPTION_T       *opt_i,
	bool_t         *s,		/* ? compute slopes	 */
	bool_t         *a,		/* ? compute aspects	 */
	int            *nbits,		/* # output bits	 */
	fpixel_t       *spacing)	/* grid spacing		 */
{
	*s = got_opt(*opt_s);
	*a = got_opt(*opt_a);

 /* default is to do both */
	if (!*s && !*a) {
		*s = *a = TRUE;
	}
 /*
  * # bits in output image
  */
	if (got_opt(*opt_i)) {
		nbits[0] = int_arg(*opt_i, 0);
		if (nbits[0] <= 0) {
			error("# bits must be >= 1");
		}
		if (n_args(*opt_i) > 1) {
			nbits[1] = int_arg(*opt_i, 1);
			if (nbits[1] <= 0) {
				error("# bits must be >= 1");
			}
		}
		else {
			nbits[1] = nbits[0];
		}
	}
	else {
		nbits[0] = nbits[1] = CHAR_BIT;
	}
 /*
  * grid spacing
  */
	if (got_opt(*opt_d)) {
		if (n_args(*opt_d) == 1) {
			spacing[0] = spacing[1] = real_arg(*opt_d, 0);
		}
		else {
			spacing[0] = real_arg(*opt_d, 0);
			spacing[1] = real_arg(*opt_d, 1);
		}
		if (spacing[0] <= 0 || spacing[1] <= 0)
			error("-d %g,%g : must be positive",
			      spacing[0], spacing[1]);
	}
	else {
		spacing[0] = spacing[1] = 0;
	}
}
