#include <stdio.h>
#include "mvalloc.h"

void            m_init();
double          m_sum();
double          sum();

void
main()
{
    double         **x;
    long             numel;

    x = MValloc(MV_DOUBLE, 2, 5L, 5L);
    numel = MVsize((void *)x, 0) * MVsize((void *)x, 1);
    m_init(x);
    printf("sum of elements is %.14g (should be %.14g)\n", 
	   m_sum(x), sum(numel-1));
    MVfree((void *)x);
}

void
m_init(x)
double **x;
{
    long             d1, d2;
    long             i, j;
    double           val = 0.0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    x[i][j] = val++;
}

double
m_sum(x)
double **x;
{
    long             d1, d2;
    long             i, j;
    double           val = 0.0;

    d1 = MVsize((void *)x, 0);
    d2 = MVsize((void *)x, 1);
    for (i = 0; i < d1; i++)
	for (j = 0; j < d2; j++)
	    val += x[i][j];
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
