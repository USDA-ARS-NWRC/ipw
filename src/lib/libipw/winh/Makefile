CATEGORY := winh
DESCRIPTION := window headers

FUNCS := winhdup winhfree winhmake winhread winhwrite

PRIVATE_SRCS := _winhcheck.c

INCLUDE_FILES := winh.h

include $(IPW)/make/funcCategory

_winhcheck.o winhdup.o winhread.o winhwrite.o : _winh.h
