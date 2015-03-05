PGM := mkpcp_a
DESCRIPTION := make precip files for 'snobal' command with adj for Ta

SRCS := main.c calc_snow.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
