PGM := ninterp
DESCRIPTION := linearly interpolate multi-band image over n-dimensional table

SRCS := headers.c interp_image.c load_table.c main.c n_interp.c vars.c

EXTRA_LIBS := image

include $(IPW)/make/command

$(OBJS) : pgm.h
