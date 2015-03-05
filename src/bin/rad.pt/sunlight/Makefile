PGM := sunlight
DESCRIPTION := sunrise and sunset times, and quadrature weights

SRCS := krontime.c main.c sunlight_opts.c vqk15.c vqk21.c vqkagn.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
