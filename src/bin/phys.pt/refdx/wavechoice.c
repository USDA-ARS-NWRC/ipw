/* wavechoice - conversion function for wavelength specification */

#include "ipw.h"
#include "pgm.h"

double	_mdiv_const;

#define NUNITS		9
#define	LIGHT_SPEED	2.99722458e8

/* Dana Jacobsen, August 92
 * Changed const to constnt since const is a keyword in ANSI C
 */
struct CHOICE {
	char	*abbrev;	/* units abbreviation */
	char	*desc;		/* units description */
	double	(*f)();		/* function to convert units to um */
	double	constnt;	/* constant to use in function */
};

static struct CHOICE	choices[NUNITS] = {
	{ "A", "Angstroms", multiply, 1.e-4 },
	{ "nm", "nanometers", multiply, 1.e-3 },
	{ "um", "micrometers (default)", NULL, 1. },
	{ "mm", "millimeters", multiply, 1.e3 },
	{ "cm", "centimeters", multiply, 1.e4 },
	{ "m", "meters", multiply, 1.e6 },
	{ "icm", "inverse centimeters", divide, 1.e4 },
	{ "MHz", "megahertz", divide, LIGHT_SPEED },
	{ "GHz", "gigahertz", divide, LIGHT_SPEED*1.e-3 }
};


void
wavechoice(
	char	*units,
	double   (**xform)(double))
{
	char	errmsg[80];
	int	i;

	*xform = NULL;

	if (units != NULL) {
		if (streq(units, "help")) {
			fprintf(stderr, "units supported are:\n");
			for (i = 0; i < NUNITS; i++)
				fprintf(stderr, "\t%s\t%s\n",
				    choices[i].abbrev, choices[i].desc);
			exit(1);
		}
		else {
			for (i = 0; i < NUNITS; i++) {
				if (streq(units, choices[i].abbrev)) {
					*xform = choices[i].f;
					_mdiv_const = choices[i].constnt;
					break;
				}
			}
			if (i == NUNITS) {
				sprintf(errmsg,
					"units %s not supported", units);
				error(errmsg);
			}
		}
	}
}

double
multiply(
	double	x)
{
	return (x * _mdiv_const);
}

double
divide(
	double	x)
{
	return (_mdiv_const/x);
}
