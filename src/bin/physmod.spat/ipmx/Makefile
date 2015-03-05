PGM := ipmx
DESCRIPTION := latent heat flux (LE) using Penman-Monteith method

SRCS := get_sample.c headers.c ipmx.c main.c newlqh.c output.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
