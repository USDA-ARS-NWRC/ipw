/*
** NAME
**	seval - evaluation of spline at m points
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int	
**	seval(
**		double	x[],
**		double	y[],
**		int	nx,
**		double	c[],
**		double	*u,
**		double	*s,
**		int	m)
**
** DESCRIPTION
**
** RETURN VALUE
**	OK	coefficients computed
**
**	ERROR	error occurred, and message stored via 'usrerr' routine
**
** HISTORY
**	June 96	Converted from QDIPS to IPW by J. Domingo, OSU
*/

#include "ipw.h"

int	
seval(
	double	x[],
	double	y[],
	int	nx,
	double	c[],
	double	*u,
	double	*s,
	int	m)
{
	double		d,dd;
	register int	i,k,nxm2;
	int		jer,ker;

	dd = 0;
	i = jer = ker = 0;
	nxm2 = nx-2;
	if (m<=0) {
		usrerr("m=%d",m);
		return(ERROR);
	}

	for (k=0; k<m; k++) {
		d = *u-x[i];
		if (d==0.)
			goto exact;

		if (d>0.) {
retry: 
			if (i>nxm2) {
				if (dd>0.)
					ker = -3;
				i = nxm2;
				d = *u-x[i];
				goto eval;
			}
			dd = *u-x[i+1];
			if (dd>=0.) {
				i++;
				d = dd;
				goto retry;
			}
			if (d==0.)
				goto exact;
			goto eval;
		}
		while (d<0) {
			if (i==0) {
				jer = -2;
				goto eval;
			}
			i--;
			d = *u-x[i];
		}
		if (d==0.)
			goto exact;
eval: 
		*s++ = ((c[2*nx+i]*d+c[nx+i])*d+c[i])*d+y[i];
		u++; 
		continue;
exact: 
		*s++ = y[i]; 
		u++; 
	}

	if (jer+ker != 0) {
		usrerr("%s","output abscissas out of range");
		return(ERROR);
	}

	return(OK);
}
