# Possible targets:
#  
# all             Build all the libraries and commands, as well as their
#                   man pages.  (default target)
# install         Install all the commands, libraries, header files, and
#                   man pages.
# tests           Run the tests to check for the correct installation
#                   of the IPW commands.
# clean           Remove all the built libraries, commands and man pages
#                   in the src/ directory, but leave any files installed
#                   in other directories alone.
# uninstall       Uninstall all the commands, libraries, header files, and
#                   man pages.
# distclean       Remove all the generated files; in addition to removing
#                   what the "clean" and "uninstall" targets do, also
#                   remove files created during configuration and testing.

.PHONY : all install tests clean uninstall distclean help funcs

all :
	$(MAKE) -C make $@
	$(MAKE) -C src $@

tests :
	$(MAKE) -C tests

install clean uninstall :
	$(MAKE) -C src $@

#  The directories must be emptied in the order listed below because the
#  Makefiles in src/, tests/ and make/ need conf/conf.mk to exist.
distclean :
	$(MAKE) -C src uninstall+clean
	$(MAKE) -C tests $@
	$(MAKE) -C make $@
	$(MAKE) -C conf $@

help :
	@sed -n 's/^#[- ]//p' Makefile
	
funcs : 
	$(MAKE) -C src libs
	$(MAKE) -C src install-libs
	$(MAKE) -C src cmds
	$(MAKE) -C src install-cmds
