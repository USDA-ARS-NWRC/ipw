/*
** NAME
**	growth - estimate snow grain growth as a function of time
** 
** SYNOPSIS
**	double growth(t)
**	double t;
** 
** DESCRIPTION
**	Second order Chebyshev polynomial, with coefs. 4,3,2,1;
**		factor = ((a + bt + t**2) / (c + dt + t**2)) - 1.0
**	(see: Acton, 1970, pgs. 292-302)
**	Function ranges from 1.0-0.0 while t ranges from 1.0-infinity;
**	80% of range has been achieved by t=9;
**	Function returns (1.0 - factor);
** 
** BUGS
**	fits measured values, but has no real physical basis...
**
*/

double
growth(
	double	t)		/* time since last snow storm (decimal days) */
{
	double	a;
	double	b;
	double	c;
	double	d;
	double	factor;

	a = 4.0;
	b = 3.0;
	c = 2.0;
	d = 1.0;

	factor = (a+(b*t)+(t*t))/(c+(d*t)+(t*t)) - 1.0;

	return (1.0 - factor);
}
