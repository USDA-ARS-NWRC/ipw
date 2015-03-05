CATEGORY := hdrio
DESCRIPTION := image header I/O

FUNCS := hcopy hdralloc hgetrec hpass hputrec hrband hrname hrskip hrvers hwprmb

PRIVATE_SRCS := _hdrioinit.c _hdriovars.c _hrrec.c _hwrec.c

INCLUDE_FILES := hdrio.h

include $(IPW)/make/funcCategory

$(OBJS) : _hdrio.h
