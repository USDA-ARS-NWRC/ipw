#  shell script to make help file for IPW's top-level src directory

cat << END
all		Make all the commands and library functions, their man
		  pages, and all the lists and indexes.  (default)
install		Install the libraries, their include files, commands, and
		  all man pages.
uninstall	Uninstall libraries, include files, commands and man pages.
clean		Clean out object code files, and local copies of executables,
		  libraries, and man pages from source directories.

cmds		Make all the commands.
install-cmds	Install all the commands.
uninstall-cmds	Uninstall all the commands.

libs		Make all the libraries.
install-libs	Install all the libraries and their include files.
uninstall-libs	Uninstall all the libraries and their include files.

man		Make all the man pages for commands and libraries.
install-man	Install the man pages for commands and libraries.
uninstall-man	Install the man pages for commands and libraries.
END
