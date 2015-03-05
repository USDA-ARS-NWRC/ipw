#  A shell script to make various help files for "command"
#
#  $1 = type of object: either "program" or "script"
#  $2 = "aux" if auxiliary related targets should be included

if [ $# -eq 2 -a -n "$2" -a "$2" = "aux" ] ; then
    aux=yes
else
    aux=no
fi

cat << END
all		Make the command, its man pages, the text and HTML
		  files with the command's name and description. (default)
END

if [ $aux = "yes" ] ; then
    cat << END
		  Also make any auxiliary scripts, programs and files.
install		Install the command, its HTML and troff man pages, and
		  any auxiliary scripts, programs and files.
END
else
    cat << END
install		Install the command, and its HTML and troff man pages.
END
fi

cat << END
cmd		Make the $1 for the command.
install-cmd	Install the command.
uninstall-cmd	Uninstall the command.
END

if [ $aux = "yes" ] ; then
    cat << END
uninstall	Uninstall the command, its HTML and troff man pages, and
		  any auxiliary scripts, programs and files.
END
else
    cat << END
uninstall	Uninstall the command, and its HTML and troff man pages.
END
fi

cat << END
clean-cmd	Remove the command.
END

if [ $1 = "program" ] ; then
    cat << END
clean-objs	Remove the command and the object files.
END
fi

if [ $1 = "script" -a $aux = "yes" ] ; then
    cat << END
clean		Remove the command, its HTML and troff man pages, and
		  any auxiliary scripts and programs.
END
elif [ $1 = "script" ] ; then
    cat << END
clean		Remove the command, and its HTML and troff man pages.
END
elif [ $aux = "yes" ] ; then
    cat << END
clean		Remove the command, its HTML and troff man pages, any
		  auxiliary scripts and programs, and any debugging versions
		  of the command and its object files.
END
else
    cat << END
clean		Remove the command, its HTML and troff man pages, and
		  any debugging versions of the command and its object files.
END
fi

if [ $1 = "program" ] ; then
    cat << END

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
fi

if [ $aux = "yes" ] ; then
    cat << END

aux-cmds	Make any auxiliary scripts and programs.
install-aux	Install auxiliary scripts, programs and files.
END
fi
 
#  A blank line
cat << END

END

sh ./_manpage.sh sample-man
sh ./_namedesc.sh command

cat << END

tar    cpio	Make a tar or cpio archive for exporting the command (local
tar.Z  cpio.Z	  commands only).  The ".Z" and ".gz" targets also compress the
tar.gz cpio.gz	  archive using "compress" and GNU's "gzip" respectively.
END
