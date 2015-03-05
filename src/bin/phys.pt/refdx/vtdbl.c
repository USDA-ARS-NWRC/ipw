/*
**	transpose double precision vector
*/

#include "ipw.h"
#include "pgm.h"

/*
 *	C translation of IMSL routine vtran
 *	'double' and 'vtdbl' must be defined for routine to work, e.g.
 *	see the routine 'vtran1.c':
 *		typedef char			double;
 *		#define vtdbl	vtran1
 *		#define sqdbl		sqtran1
 */

void
vtdbl(
	double	*zot,
	int	m,
	int	n)
{
	int	iexp[13];
	int	ipf[13];
	int	ipwr[13];
	int	itemp[13];

	int	i;
	int	id;
	int	isd;
	int	istart;
	int	it;
	int	j;
	int	k;
	int	kk;
	int	l;
	int	ll;
	int	nm;
	int	nmis;
	int	nmo2;
	int	npf;

	register double	*a;
	double		temp1;
	double		temp;

	/* if matrix is one column or one row return */
	if (n<=1 || m<=1)
		return;

	/* square matrices are handled separately */
	if (n == m)
		sqdbl(zot, m);
	
	else {

		/* fortran indexing */
		a = &zot[-1];

		nm = n*m - 1;

		/*
		 *	matrix is not square.
		 *	Transposition of an n by m matrix amounts to
		 *	replacing the element at vector position i with
		 *	the element at position n*i (mod n*m-1). Each
		 *	subcycle of this permutation is completed in
		 *	order.
		 */

		/* decompose nm into its prime factors */
		npf = vdcps(nm, ipf, iexp, ipwr);
		for (i = 0; i < npf; i++)
			itemp[i] = 0;

		/* generate divisors of nm less than nm/2 */
		id = 1;
		nmo2 = nm/2;
		while (id < nmo2) {
			j = nm/id;
			for (i = 0; i < npf; i++)
				if (itemp[i] != iexp[i]) {
					j /= ipf[i];
					j *= ipf[i]-1;
				}
			/*
			 *	the starting point of a subcycle is divisible
			 *	only by id and must not appear in any other
			 *	subcycle
			 */
			istart = id;
			do {
				nmis = nm-istart;
				if (istart != id) {
					isd = istart/id;
					for (i = 0; i < npf; i++)
						if (itemp[i] != iexp[i] &&
						    (isd % ipf[i]) == 0)
							goto l20;
					it = istart;
					do {
						/* it = (n*it) % nm; */
						it = modprod(n, it, nm);
						if (it<istart || it>nmis)
							goto l20;
					}
					while (it>istart && it<nmis);
				}
				temp = a[istart+1];
				temp1 = a[nmis+1];
				k = istart;
				for (;;) {
					/* kk = (n*k) % nm; */
					kk = modprod(n, k, nm);
					l = nm-k;
					ll = nm-kk;
					j -= 2;
					/*
					**	Move two elements.  The
					**	second from the negative
					**	subcycle.  Check to see if
					**	the subcycle is complete.
					*/
					if (kk==istart)
						goto l10;
					if (ll==istart)
						break;
					/*
					**	subcycle not complete.  move
					**	elements and update pointers
					*/
					a[k+1] = a[kk+1];
					a[l+1] = a[ll+1];
					k = kk;
				}
				a[k+1] = temp1;
				a[l+1] = temp;
				goto l20;

				/* subcycle complete. recompute id */
l10:
				a[k+1] = temp;
				a[l+1] = temp1;
l20:
				istart += id;
			}
			while (j>0);

			for (i = 0; i < npf; i++) {
				if (itemp[i] != iexp[i])
					goto l30;
				itemp[i] = 0;
				id /= ipwr[i];
			}
			break;
l30:
			itemp[i] = itemp[i]+1;
			id *= ipf[i];
		}
	}
}

/* in-place transpose of a square matrix */

void
sqdbl(
	double	*zot,
	int	n)
{
	register double	*a;
	double		temp;

	register	i;
	register	j;
	int		k;
	register	l;
	int		nm1;
	int		nm;
	int		np1;

	/* Fortran indexing */
	a = &zot[-1];

	k = 2;
	np1 = n + 1;
	nm1 = n - 1;
	nm = n * n;
	for (i = n; i <= nm; i += n) {
		l = k + nm1;
		for (j = k; j <= i; j++) {
			temp = a[j];
			a[j] = a[l];
			a[l] = temp;
			l += n;
		}
		k += np1;
	}	
}
