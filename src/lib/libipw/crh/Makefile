CATEGORY := crh
DESCRIPTION := class range headers

FUNCS := crh crhdup crhmake crhread crhsort crhwrite

PRIVATE_SRCS := _crharrays.c _crhcheck.c _crhinit.c _crhvars.c

INCLUDE_FILES := crh.h

include $(IPW)/make/funcCategory

$(OBJS) : _crh.h
