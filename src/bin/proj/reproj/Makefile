PGM := reproj
DESCRIPTION := reproject an IPW image to a new geographic projection

SRCS := bilinear.c cubic.c headers.c main.c nearest.c nothing.c reproj.c

EXTRA_LIBS := gctp

include $(IPW)/make/command

$(OBJS) : pgm.h
