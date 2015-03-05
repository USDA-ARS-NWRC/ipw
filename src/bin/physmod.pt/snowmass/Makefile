PGM := snowmass
DESCRIPTION := simple snowmelt model based on daily radiation

SRCS := main.c melt.c penman.c rainint.c snowint.c

EXTRA_LIBS := model

include $(IPW)/make/command

$(OBJS) : pgm.h
