#include "ipw.h"
#include "pgm.h"

#define	ADD	"add"
#define	SUB	"sub"
#define	MULT	"mult"
#define	DIV	"div"
#define	POW	"pow"

#define SQRT	"sqrt"
#define LN	"ln"
#define EXP	"exp"
#define LOG	"log"
#define POW10	"10x"

int
main(
	int             argc,
	char          **argv)
{
	char		 *op;
	char		 aop;
	float		 val = 0.0; /** initialize val to floating point NULL **/

	static OPTION_T opt_o = {
		'o', "arithmetic operators: (add, sub, mult, div, pow)\n\t\texponential operators: (sqrt, ln, exp, log, 10x)",
		STR_OPTARGS, "aop",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_v = {
		'v', "image scaling constant",
		REAL_OPTARGS, "val",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "no data value (for masked pixels)",
		REAL_OPTARGS, "no_data",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_o,
		&opt_v,
		&opt_m,
		&opt_n,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operands, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * access mask image
  */
	if (got_opt(opt_m)) {
		parm.m_fd = uropen(str_arg(opt_m, 0));
		if (parm.m_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		parm.m_fd = ERROR;
	}
 /*
  * set no data value
  */
	if (got_opt(opt_n)) {
		if (!got_opt(opt_m)) {
			warn ("no mask provided; -n option ignored");
		} else {
			parm.no_data = real_arg (opt_n, 0);
			parm.got_no_data = TRUE;
		}
	} else {
		parm.got_no_data = FALSE;
	}
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * process arguments
  */
	op = str_arg(opt_o, 0);
/*
 * set operator for switch
 */
       
	if (streq(op,ADD)) {
		aop = OP1;
		if ((val = real_arg(opt_v, 0)) == ERROR)
			error("-v option required for add operator");
	}
	else if (streq(op,SUB)) {
		aop = OP2;
		if ((val = real_arg(opt_v, 0)) == ERROR)
			error("-v option required for sub operator");
	}
	else if (streq(op,MULT)) {
		aop = OP3;
		if ((val = real_arg(opt_v, 0)) == ERROR)
			error("-v option required for mult operator");
	}
	else if (streq(op,DIV)) {
		aop = OP4;
		if ((val = real_arg(opt_v, 0)) == ERROR)
			error("-v option required for div operator");
	}
	else if (streq(op,POW)) {
		aop = OP5;
		if ((val = real_arg(opt_v, 0)) == ERROR)
			error("-v option required for pow operator");
	}
	else if (streq(op,SQRT)) {
		aop = OP6;
		if (got_opt(opt_v))
			warn ("-v option ignored with sqrt operator");
	}
	else if (streq(op,LN)) {
		aop = OP7;
		if (got_opt(opt_v))
			warn ("-v option ignored with ln operator");
	}
	else if (streq(op,EXP)) {
		aop = OP8;
		if (got_opt(opt_v))
			warn ("-v option ignored with exp operator");
	}
	else if (streq(op,LOG)) {
		aop = OP9;
		if (got_opt(opt_v))
			warn ("-v option ignored with log operator");
	}
	else if (streq(op,POW10)) {
		aop = OP10;
		if (got_opt(opt_v))
			warn ("-v option ignored with 10x operator");
	}
	else {
		error("Bad arithmetic operator \"%s\"", op);
		usage();
	}

 /*
  * do it
  */
	headers(FALSE);

	if (parm.m_fd != ERROR || aop == '^' || aop == 'q' || aop == 'l' ||
		aop == 'e' || aop == 'g' || aop == 'x') {
		scale(aop, val);
		headers(TRUE);
		output();
 /*
  * if there is no mask and operator is +,-,* or / by constant,
  * 	no need to scale the data - just apply the scale to the LQ header
  *	and make a copy of the raw image
  */
	} else {
		fast_scale (aop, val);
	}
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
