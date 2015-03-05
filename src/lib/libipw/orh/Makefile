CATEGORY := orh
DESCRIPTION := orientation headers

FUNCS := orhdup orhfree orhmake orhread orhwrite

PRIVATE_SRCS := _orhcheck.c

INCLUDE_FILES := orh.h

include $(IPW)/make/funcCategory

_orhcheck.o orhdup.o orhread.o orhwrite.o : _orh.h
