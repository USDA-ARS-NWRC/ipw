PGM := mkprecip
DESCRIPTION := make precipitation images for 'isnobal' command

SRCS := calc_snow.c globals.c headers.c main.c mkprecip.c

EXTRA_LIBS := image

include $(IPW)/make/command

$(OBJS) : pgm.h
