#include <stdio.h>
#include "mvalloc.h"

void            m_init();
int          m_sum();
int          sum();

void
main()
{
    int         *****x;
    long                numel;

    x = MValloc(MV_DOUBLE, 5, 3L, 4L, 5L, 6L, 7L);
    numel = MVsize((void *)x, 0) * MVsize((void *)x, 1) *
	    MVsize((void *)x, 2) * MVsize((void *)x, 3) *
	    MVsize((void *)x, 4);
    m_init(x);
    printf("sum of elements is %d (should be %d)\n", 
	   m_sum(x), sum(numel-1));
    MVfree((void *)x);
}

void
m_init(x)
int *****x;
{
    long             d1, d2, d3, d4, d5;
    long             i, j, k, l, m;
    int           val = 0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    d3 = MVsize((void *)x, 2);
    d4 = MVsize((void *)x, 3);
    d5 = MVsize((void *)x, 4);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    for (k = 0; k < d3; k++)
	        for (l = 0; l < d4; l++)
	            for (m = 0; m < d5; m++)
	                x[i][j][k][l][m] = val++;
}

int
m_sum(x)
int *****x;
{
    long             d1, d2, d3, d4, d5;
    long             i, j, k, l, m;
    int           val = 0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    d3 = MVsize((void *)x, 2);
    d4 = MVsize((void *)x, 3);
    d5 = MVsize((void *)x, 4);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    for (k = 0; k < d3; k++)
	        for (l = 0; l < d4; l++)
	            for (m = 0; m < d5; m++)
	                val += x[i][j][k][l][m];
    return val;
}

int
sum(i)
long i;
{
    int val = 0;

    while (i > 0)
	val += i--;
    return val;
}
