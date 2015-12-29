PGM := horizon
DESCRIPTION := find angles to local horizon along rows of elevation file

SRCS := azmf.c bihvalid.c headers.c hor1b.c hor1f.c hor1d.c hormask.c \
	horval.c main.c newlqh.c parm.c zenf.c horizon.c ../../transform/transpose/ximg.c \
	../../transform/skew/skew.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
