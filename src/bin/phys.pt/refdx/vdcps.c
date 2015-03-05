/* prime factor decomposition */

#include "ipw.h"
#include "pgm.h"

int
vdcps(
	int	n,
	int	*ipf,
	int	*iexp,
	int	*ipwr)
{
	int	i;
	int	id;
	int	ifc;
	int	iq;
	int	nf;

	/* decrement pointers so Fortran indexing can be used */
	--ipf;
	--iexp;
	--ipwr;

	i = 0;
	nf = (n >= 0) ? n : -n;
	if (nf>1) {
		ifc = 0;
		id = 2;
		for (;;) {
			iq = nf/id;
			if (nf==id*iq) {
				nf = iq;
				if (id<=ifc) {
					ipwr[i] *= id;
					iexp[i]++;
				}
				else {
					i++;
					ipf[i] = id;
					ipwr[i] = id;
					iexp[i] = 1;
					ifc = id;
				}
			}
			else {
				if (iq<=id)
					break;
				if (id>2)
					id += 2;
				else
					id = 3;
			}
		}
		if (nf>1) {
			i++;
			ipf[i] = nf;
			ipwr[i] = nf;
			iexp[i] = 1;
		}
	}
	return(i);
}
