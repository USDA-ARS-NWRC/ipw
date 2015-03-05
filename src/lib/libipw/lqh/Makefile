CATEGORY := lqh
DESCRIPTION := linear quantization headers

FUNCS := lqh lqhdup lqhmake lqhread lqhwrite

PRIVATE_SRCS := _lqharrays.c _lqhcheck.c _lqhfree.c _lqhinit.c _lqhinterp.c \
		_lqhvars.c

INCLUDE_FILES := lqh.h

include $(IPW)/make/funcCategory

$(OBJS) : _lqh.h
