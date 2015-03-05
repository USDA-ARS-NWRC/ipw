PGM := dem2ipw
DESCRIPTION := convert USGS DEM to IPW image

SRCS := atoid.c bigatof.c demhdr.c fillvec.c headers.c litatof.c main.c \
	output.c parm.c prdemhdr.c profhdr.c profiles.c proj.c prprofhdr.c \
	units.c

include $(IPW)/make/command

$(OBJS) : pgm.h
