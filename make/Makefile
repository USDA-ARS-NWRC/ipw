#  This makefile creates the scripts used by various rules in IPW makefiles

include _macros

#  Make the macro IPW_MACRO_FILE empty, so the macro "mk-script" will not
#  look for a local copy of the file "IPWmacros"
IPW_MACRO_FILE :=

.PHONY : all scripts help-files
all : scripts help-files

#  The "ipwMacro" script depends upon the master configuration file
bin/ipwMacro : src/ipwMacro.pl $(IPW_CONF_MAKE)
	printf "#! $(PERL)\n" | cat -  $< > $@
	chmod $(SCRIPT_MODE) $@

SH_SCRIPTS := install.sh mkManDirs.sh
PERL_SCRIPTS := modDate.pl html2um.pl nameDesc.pl makeIndex.pl

INSTALLED_SH_SCRIPTS := $(foreach script, $(SH_SCRIPTS:.sh=), bin/$(script))
INSTALLED_PERL_SCRIPTS := $(foreach script, $(PERL_SCRIPTS:.pl=), bin/$(script))

scripts : bin/ipwMacro $(INSTALLED_SH_SCRIPTS) $(INSTALLED_PERL_SCRIPTS)

#  sh scripts
$(INSTALLED_SH_SCRIPTS) : bin/% : src/%.sh
	$(mk-script)

#  perl scripts
$(INSTALLED_PERL_SCRIPTS) : bin/% : src/%.pl
	$(mk-script)


help-files :
	$(MAKE) -C Help

distclean :
	rm -f bin/*
	rm -f Help/*.hlp
