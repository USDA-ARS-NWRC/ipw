#  A shell script to make help file for "aux_pgm"
#

cat << END
pgm		Make the auxiliary program (default).
install		Install the program in the auxiliary directory.
uninstall	Uninstall the program.

clean-pgm	Remove the program.
clean-objs	Remove the program and its object files.
clean		Remove the program, its object files, and any debugging
		  versions of the program and their object files.

debug		Make a debugging version of the program in the
		  subdirectory "Debug" with non-debugging libraries.
debug-all	Make a debugging version of the program with the
		  extension ".all" in the subdirectory "Debug" with
		  debugging libraries.
clean-debug	Remove the debugging versions of the program and their
		  object files.
clean-debug-cmd	Remove the debugging versions of the program
clean-debug-obj	Remove the debugging versions of program's object files.

END
