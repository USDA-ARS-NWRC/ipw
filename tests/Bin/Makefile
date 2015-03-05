#  This makefile creates the scripts used by tests in the parent
#  directory

include $(IPW)/make/_macros

SH_SCRIPTS := cmpverb.sh striphist.sh
PERL_SCRIPTS := le2be.pl testdeltas.pl

SCRIPTS := $(basename $(SH_SCRIPTS) $(PERL_SCRIPTS))

#  Default target is to create scripts

.PHONY : scripts clean

scripts : $(SCRIPTS)

cmpverb.sh : cmpverb-src.sh
	@printf "Making \"$@\"\n"
	@$(PERL) set-dolitbig.pl < $< > $@

$(SH_SCRIPTS:.sh=) : % : %.sh
	$(mk-script)
$(PERL_SCRIPTS:.pl=) : % : %.pl
	$(mk-script)

clean :
	rm -f $(SCRIPTS)
	rm -f cmpverb.sh
