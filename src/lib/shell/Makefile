#  This is the Makefile for shell-library commands -- those commands
#  that are used by IPW scripts.


SCRIPTS := isposint sherror usage

include $(IPW)/make/_macros
include $(IPW)/make/_libmacros

.PHONY : default
default : all

include $(IPW)/make/_directory

.PHONY : all library scripts funcs add-funcs delete-funcs install install-lib
.PHONY : man install-man uninstall-man clean-man
.PHONY : clean uninstall uninstall-lib clean.DIRS
.PHONY : uninstall+clean uninstall+clean.DIRS help

all : library man list

SCRIPTS_IN_BUILD_DIR := $(foreach script,$(SCRIPTS),$(LIB_BUILD_DIR)/$(script))
library : $(SCRIPTS_IN_BUILD_DIR)

$(SCRIPTS_IN_BUILD_DIR) : $(LIB_BUILD_DIR)/% : %
	$(INSTALL) -c -m $(SCRIPT_MODE) $< $@

funcs scripts : $(SCRIPTS)

$(SCRIPTS) : % : %.sh
	$(mk-script)

#  --------------------------------------------------------------------------

#  These targets do nothing but are defined for consistency with compiled
#  libraries

add-funcs delete-funcs :

#  --------------------------------------------------------------------------

#  Target to build HTML list of shell-library commands

.PHONY : list clean-list

list : $(CMD_LIST_HTML)

NAME_DESC_FILES := $(foreach script,$(SCRIPTS),Man/$(script)/$(NAME_DESC_HTML))

$(CMD_LIST_HTML) : $(NAME_DESC_FILES)
	@printf "Making file \"$(@)\"\n"
	@cat Header.html $^ Footer.html > $@

$(NAME_DESC_FILES) : Man/%/$(NAME_DESC_HTML) : Man/%/Makefile
	$(MAKE) -C $(dir $@) $(NAME_DESC_HTML)

clean-list :
	rm -f $(CMD_LIST_HTML)

#  --------------------------------------------------------------------------

#  Targets to install

INSTALLED_SCRIPTS := $(foreach script, $(SCRIPTS), $(LIB_INSTALL_DIR)/$(script))

install-scripts install-lib : $(INSTALLED_SCRIPTS)

$(INSTALLED_SCRIPTS) : $(LIB_INSTALL_DIR)/% : %
	$(INSTALL) -c -m $(SCRIPT_MODE) $< $@

install : install-scripts install-man

uninstall : uninstall-scripts uninstall-man

uninstall-scripts uninstall-lib :
	rm -f $(INSTALLED_SCRIPTS)

uninstall+clean : uninstall-scripts

#  --------------------------------------------------------------------------

man install-man uninstall-man clean-man clean.DIRS uninstall+clean.DIRS :
	@target=$(@:.DIRS=) ; subdirs='$(foreach s, $(SCRIPTS), Man/$(s))' ; \
	    $(recurse-on-subdirs)

#  --------------------------------------------------------------------------

clean uninstall+clean : % : clean-scripts clean-list %.DIRS
	rm -f $(SCRIPTS_IN_BUILD_DIR)

.PHONY : clean-scripts
clean-scripts :
	rm -f $(SCRIPTS)

help :
	@ $(PAGER) Makefile.hlp

#  --------------------------------------------------------------------------

include $(IPW)/make/_rules
