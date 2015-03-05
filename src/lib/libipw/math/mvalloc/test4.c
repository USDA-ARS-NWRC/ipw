#include <stdio.h>
#include "mvalloc.h"

void            m_init();
double          m_sum();
double          sum();

void
main()
{
    double         ****x;
    long               numel;

    x = MValloc(MV_DOUBLE, 4, 5L, 5L, 5L, 5L);
    numel = MVsize((void *)x, 0) * MVsize((void *)x, 1) *
	    MVsize((void *)x, 2) * MVsize((void *)x, 3);
    m_init(x);
    printf("sum of elements is %.14g (should be %.14g)\n", 
	   m_sum(x), sum(numel-1));
    MVfree((void *)x);
}

void
m_init(x)
double ****x;
{
    long             d1, d2, d3, d4;
    long             i, j, k, l;
    double           val = 0.0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    d3 = MVsize((void *)x, 2);
    d4 = MVsize((void *)x, 3);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    for (k = 0; k < d3; k++)
	        for (l = 0; l < d4; l++)
	            x[i][j][k][l] = val++;
}

double
m_sum(x)
double ****x;
{
    long             d1, d2, d3, d4;
    long             i, j, k, l;
    double           val = 0.0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    d3 = MVsize((void *)x, 2);
    d4 = MVsize((void *)x, 3);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    for (k = 0; k < d3; k++)
	        for (l = 0; l < d4; l++)
	            val += x[i][j][k][l];
    return val;
}

double
sum(i)
long i;
{
    double val = 0.0;

    while (i > 0)
	val += i--;
    return val;
}
