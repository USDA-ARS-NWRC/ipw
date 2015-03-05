#include	<unistd.h>
#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"
#include	"pgm.h"


int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_p = {
		'p', "input file with density profile info",
		STR_OPTARGS, "density-profile",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "output summary of density profile",
	};

	static OPTION_T *optv[] = {
		&opt_p,
		&opt_s,
		0
	};

	char	   *prof;	/* snow density profile file name */

	int	   i;		/* index */
	int	   np;		/* number of profile points */

	short	   bottom;	/* flag for location of layer bottom */
	short	   in;		/* flag for stdin re-direct */
	short	   out;		/* flag for stdout re-direct */
	short	   summ;	/* flag for output of profile summary */
	short	   top;		/* flag for location of layer top */

	double	   z[500];	/* profile density measurement heights */
	double	   rho[500];	/* profile densities */
	double	   swe[500];	/* profile SWE's */

	double	   layer_swe;	/* calculated layer swe */
	double	   rho_bar;	/* average profile density */
	double	   total_swe;	/* sum of swe[i] */
	double	   zl;		/* working profile thickness */
	double	   z0;		/* working layer thickness */
	double	   z_top;	/* upper layer boundary */
	double	   z_bottom;	/* lower layer boundary */

	FILE *     profile;


	/*	get args from command line	*/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

	prof = str_arg(opt_p, 0);
	summ  = got_opt(opt_s);

	/*	open snow density profile file	*/
	if ((profile = fopen(prof, "r")) == NULL)
		error("can't open density profile file '%s'", prof);

	/*	read snow density profile file	*/
	i = 0;
	while (fscanf(profile, "%lf %lf", &z[i], &rho[i]) > 0)
		i++;
	np = i;

	/*	check z and rho, and set swe profile	*/
	total_swe = 0.0;
	if (summ) {
		printf("Density Profile Summary\n\n");
		printf("depth\tdensity\tSWE\n");
	}
	for (i=0; i<np; i++) {
		if (i == (np -1)) {
			if ((rho[i] > RHO_ICE) || (rho[i] < 0.0))
				error("bad input rho:  %g\n", rho[i]);
			if (z[i] < 0.0)
				error("bad input z:  %g\n", z[i]);
			swe[i] = z[i] * (rho[i] / RHO_W0);
		}
		else {
			if ((rho[i] > RHO_ICE) || (rho[i] < 0.0))
				error("bad input rho:  %g\n", rho[i]);
			if (z[i] < 0.0)
				error("bad input z:  %g\n", z[i]);
			if ((z[i]-z[i+1]) < 0.0)
				error("z not in decending order");
			swe[i] = (z[i] - z[i+1]) * (rho[i] / RHO_W0);
		}
		total_swe += swe[i];
		if (summ)
			printf("%.3f\t%.1f\t%.3f\n", z[i], rho[i], swe[i]);
	}
	rho_bar = 0.0;
	for (i=0; i<np; i++)
		rho_bar += (swe[i] / total_swe) * rho[i];

	if (summ)
		printf("\n%.3f\t%.1f\t%.3f\n\n", z[0], rho_bar, total_swe);

	/*	check for re-direct of stdin and stdout	*/
	in = isatty(STDIN_FILENO);
	out = isatty(STDOUT_FILENO);

	/*	get input layer thickness from stdin	*/
	if (in)
		fprintf(stderr, "Enter upper & lower layer bounds:\n");
	if (out) {
		printf("\nLayer Calculations\n");
		printf(" top  \tbottom\t SWE  \n");
	}

	while (scanf("%lf %lf", &z_top, &z_bottom) != EOF) {
		/* check inputs */
		if (	(z_top > z[0]) ||
			(z_top <= 0.0) ||
			(z_top <= z_bottom) ||
			(z_bottom < 0.0)	) {
			printf("Bad input layer boundaries\n");
			if (in)
				fprintf(stderr, "try again\n");
			continue;
		}
		/*	calculate layer swe	*/
		layer_swe = 0.0;
		i=0;
		top = bottom = FALSE;
		while (top == FALSE) {
			if (	(z_top <= z[i]) &&
				(z_bottom >= z[i+1])	) {
				zl = z[i] - z[i+1];
				z0 = z_top - z_bottom;
				layer_swe = (z0 / zl) * swe[i];
				top = TRUE;
				bottom = TRUE;
			}
			else if (z_top == z[i]) {
				layer_swe += swe[i];
				top = TRUE;
			}
			else if (z_top > z[i+1]) {
				zl = z[i] - z[i+1];
				z0 = z_top - z[i+1];
				layer_swe += (z0 / zl) * swe[i];
				top = TRUE;
			}
			i++;
		}
		while (bottom == FALSE) {
			if (z_bottom == z[i+1]) {
				layer_swe += swe[i];
				bottom = TRUE;
			}
			else if (z_bottom > z[i+1]) {
				zl = z[i] - z[i+1];
				z0 = z[i] - z_bottom;
				layer_swe += (z0 / zl) * swe[i];
				bottom = TRUE;
			}
			else
				layer_swe += swe[i];
			i++;
		}
		/*	output result	*/
		printf("%.3f\t%.3f\t%.3f\n", z_top, z_bottom, layer_swe);
	}

	ipwexit(EXIT_SUCCESS);
}
