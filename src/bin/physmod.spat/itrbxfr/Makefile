PGM := itrbxfr
DESCRIPTION := turbulent transfer (H and LE) using Brutsaert's method

SRCS := bihvalid.c get_sample.c headers.c itrbxfr.c main.c newlqh.c output.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
