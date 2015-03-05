CATEGORY := bih
DESCRIPTION := basic image headers

FUNCS := bihmake bihdup bihread bihwrite bih hbyteorder \
	 hnlines hnsamps hnbands hnbits hnbytes hostorder \
	 imgsize sampsize no_history

PRIVATE_SRCS := _bihcheck.c _bihfree.c _bihinit.c _bihvars.c

INCLUDE_FILES := bih.h

include $(IPW)/make/funcCategory

$(OBJS) : _bih.h
