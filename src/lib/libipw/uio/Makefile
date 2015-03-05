CATEGORY := uio
DESCRIPTION := unstructured I/O on images

FUNCS := ubof uclose ucopy ueof ufilename ugets uputs uread uremove uropen urskip ustdin ustdout uwflush uwopen uwopen_temp uwrite mktemplate

PRIVATE_SRCS := _getnumfds.c _ugetlvl.c _uioexit.c _uioinit.c _uioopen.c _uiovars.c _uread.c _usetlvl.c _uwrite.c

include $(IPW)/make/funcCategory

$(OBJS) : _uio.h
