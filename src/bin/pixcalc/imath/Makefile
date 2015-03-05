PGM := imath
DESCRIPTION := calculate an arithmetic expression over each sample's pixels

CATEGORY := (test)

SRCS := main.c headers.c imath.c ordinal.c globals.c \
	compile.c expression.c token.c command.c cmdbuf.c

STRING_OPTIONS := 1

EXTRA_LIBS := image

include $(IPW)/make/command

main.o imath.o expression.o : command.h token.h

token.h : tokentypes.h
	touch $@

token.o : tokentypes.c token.h

tokentypes.c tokentypes.h : tokentypes.in
	$(PERL) makeCode.pl tokentypes.h tokentypes.c < $^

command.h : cmdcodes.h
	touch $@

command.o : cmdcodes.c

cmdcodes.c cmdcodes.h : cmdcodes.in
	$(PERL) makeCode.pl cmdcodes.h cmdcodes.c < $^
