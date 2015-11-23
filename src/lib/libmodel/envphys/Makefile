CATEGORY := envphys
DESCRIPTION := environmental physics

FUNCS := bevap bowen budyer budyer2 dew_point dew_pointp efcon evap heat_stor hle1 \
	 psychrom wetbulb ri_no sati sati_mod satw ssxfr

PRIVATE_SRCS := _bdinit.c

INCLUDE_FILES := envphys.h _physunits.h

include $(IPW)/make/funcCategory

budyer.o budyer2.o _bdinit.o : _envphys.h
