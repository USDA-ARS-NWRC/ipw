#ifndef DOZIER_H
#define DOZIER_H

extern double   EXFUN(betanaught, (double u0, double g));
extern void     EXFUN(delted, (double *omega, double *g, double *tau));
extern void     EXFUN(mwgamma, (double u0, double w, double g, double *gam,
			int pick));
extern double   EXFUN(planck, (double lambda, double temp));
extern int      EXFUN(twostream, (double *gamma, double omega, double mu0,
			double tau, double r0, double *refl, double *trans,
                        double *btrans));
#endif
