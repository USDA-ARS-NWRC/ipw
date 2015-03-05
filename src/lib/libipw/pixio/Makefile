CATEGORY := pixio
DESCRIPTION := integer pixel I/O

FUNCS := pvread pvwrite pxclose

PRIVATE_SRCS := _piocbinit.c _pixioinit.c _pixiovars.c

INCLUDE_FILES := pixio.h

include $(IPW)/make/funcCategory

$(OBJS) : _pixio.h
