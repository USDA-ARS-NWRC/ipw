PGM := iwbt
DESCRIPTION := wet or ice bulb temperature from elevation, air and dewpoint temperature

SRCS := headers.c iwbt.c main.c newlqh.c output.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
