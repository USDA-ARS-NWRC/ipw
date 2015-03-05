CATEGORY := main
DESCRIPTION := command options, program startup and wrapup

FUNCS := ipwenter ipwexit opt_check usage

PRIVATE_SRCS := _cmdline.c _cvtoptarg.c _getargs.c _mainvars.c

include $(IPW)/make/funcCategory

$(OBJS) : _ipwmain.h
