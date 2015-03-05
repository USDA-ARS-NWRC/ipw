#include "ipw.h"
#include "pgm.h"

#define DEFAULT_FMT_STR "%Y_%m_%d+%H:%M:%S"

#define EXIT_TRUE	0
#define EXIT_FALSE	1

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T operands = {
		OPERAND, "date-time1 operator date-time2",
		STR_OPERANDS, "",
		REQUIRED, 3, 3
	};
	static OPTION_T *optv[] = {
		&operands,
		0
	};

	datetime_t      dt1;		/* first date-time		*/
	datetime_t      dt2;		/* second date-time		*/
	char 	       *op;		/* relational operator		*/
	int		status;		/* exit status			*/

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	if (! str2dt(str_opnd(operands, 0), DEFAULT_FMT_STR, &dt1)) {
		error ("First date-time is invalid");
	}
	if (! str2dt(str_opnd(operands, 2), DEFAULT_FMT_STR, &dt2)) {
		error ("Second date-time is invalid");
	}

	op = str_opnd(operands, 1);
	if (STREQ(op, "eq")) {
		status = STREQ(str_opnd(operands, 0), str_opnd(operands, 2))
				? EXIT_TRUE : EXIT_FALSE;
	}
	else if (STREQ(op, "ne")) {
		status = STREQ(str_opnd(operands, 0), str_opnd(operands, 2))
				? EXIT_FALSE : EXIT_TRUE;
	}
	else if (STREQ(op, "le")) {
		status = dt_in_order(&dt1, &dt2) ? EXIT_TRUE : EXIT_FALSE;
	}
	else if (STREQ(op, "gt")) {
		status = (! dt_in_order(&dt1, &dt2)) ? EXIT_TRUE : EXIT_FALSE;
	}
	else if (STREQ(op, "ge")) {
		status = dt_in_order(&dt2, &dt1) ? EXIT_TRUE : EXIT_FALSE;
	}
	else if (STREQ(op, "lt")) {
		status = (! dt_in_order(&dt2, &dt1)) ? EXIT_TRUE : EXIT_FALSE;
	}
	else {
		error("Unknown operator: %s", op);
	}

	ipwexit (status);
}
