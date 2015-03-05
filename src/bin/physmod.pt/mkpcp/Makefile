PGM := mkpcp
DESCRIPTION := make precipitation files for 'snobal' command

SRCS := main.c calc_snow.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
