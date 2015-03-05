#  shell script to make help file for "funcManpage"

cat << END
all		Make the HTML and troff man pages, and the text and HTML
		  files with the function's name and description (default).
install		Same as "install-man" target.
clean		Same as "clean-man" target.
uninstall	Same as "uninstall-man" target.

list-of-links	Print a list of other functions in this category as HTML
		  links (for the SEE ALSO section).
 
END

sh ./_manpage.sh sample-man
sh ./_namedesc.sh function
