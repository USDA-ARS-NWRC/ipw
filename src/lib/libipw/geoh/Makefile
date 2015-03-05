CATEGORY := geoh
DESCRIPTION := geodetic headers

FUNCS := geohdup geohfree geohmake geohread geohspace geohwrite

PRIVATE_SRCS := _geohcheck.c

INCLUDE_FILES := geoh.h

include $(IPW)/make/funcCategory

_geohcheck.o geohdup.o geohread.o geohwrite.o : _geoh.h
