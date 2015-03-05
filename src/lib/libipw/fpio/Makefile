CATEGORY := fpio
DESCRIPTION := floating-point pixel I/O

FUNCS := fpclose fpfmax fpfmin fphdrs fpmap fpmaplen fpvread fpvwrite mnxfp

PRIVATE_SRCS := _fpiocbinit.c _fpioinit.c _fpiovars.c

INCLUDE_FILES := fpio.h

include $(IPW)/make/funcCategory
 
$(OBJS) : _fpio.h
