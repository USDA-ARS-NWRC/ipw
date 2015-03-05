PGM := ipw2ppm
DESCRIPTION := convert IPW image to Portable Pixel Map (PPM) image

SRCS := headers.c initcolors.c ipw3ppm.c ipwload.c main.c \
	pgmwrite.c ppmwrite.c readcolormap.c rgblookup.c

include $(IPW)/make/command

$(OBJS) : pgm.h
