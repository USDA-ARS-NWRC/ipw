/*
 * generalize reverse function
 */

void
func(
	register mtype	*vec,		/* -> beginning of vector	*/
	register int	n)		/* length of vector		*/
{
	register mtype	*end;		/* -> last element		*/
	register mtype	temp;

	end = &vec[n-1];
	while (end > vec) {
		temp = *end;
		*end-- = *vec;
		*vec++ = temp;
	}
}
