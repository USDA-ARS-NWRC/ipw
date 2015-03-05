PGM := refdx
DESCRIPTION := refractive index from wavelengths

SRCS := akcoef.c main.c modprod.c refh2o.c refice.c spval1.c vdcps.c vtdbl.c \
	wavechoice.c

include $(IPW)/make/command

refh2o.o:	genref.cpp genreft.cpp waterdata.h
refice.o:	genref.cpp genreft.cpp icedata.h

$(OBJS) : pgm.h
