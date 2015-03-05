CATEGORY := main
DESCRIPTION := command options, program startup and wrapup

FUNCS := ipwenter opt_check usage

PRIVATE_SRCS := _cvtoptarg.c _getargs.c _mainvars.c

STRING_OPTIONS := 1

NO_MANPAGES := 1

include $(IPW)/make/funcCategory

$(OBJS) : _ipwmain.h
