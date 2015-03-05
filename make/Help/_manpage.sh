#  shell script to make help file for "_manpage"
#  $1 = "sample-man" if target "sample-man" should be included

cat << END
man		Make the HTML and troff versions of the man page.
install-man	Install the HTML and troff man pages.
END

if [ "$1" = "sample-man" ] ; then
cat << END
sample-man	Copy a sample man page into the current directory as an
		  initial version of the man page.
END
fi

cat << END
clean-man	Remove the HTML and troff versions of the man page.
uninstall-man	Uninstall the HTML and troff versions of the man page.
END
