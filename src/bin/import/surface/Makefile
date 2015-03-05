PGM := surface
DESCRIPTION := make IPW image by spatially interpolating irregular point data

SRCS := find_neighbors.c get_points.c headers.c interp.c main.c surface.c

include $(IPW)/make/command

$(OBJS) : pgm.h
