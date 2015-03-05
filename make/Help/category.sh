#  shell script to make help file for "category"

cat << END
all		Make all the commands, their man pages and the lists of
		  of commands (default).
cmd		Make all the commands in this category.
install-cmd	Make and install all the commands.
install		Install all the commands, their man pages, and any auxiliary
		  scripts and files.

man		Make the HTML and troff man pages for all the commands.
install-man	Install the HTML and troff man pages.
uninstall-man	Uninstall the HTML and troff man pages.
clean-man	Remove the HTML and troff man pages from the directories
		  where man pages are made.
cmd-list	Make or update the text and HTML files with the list of the
		  commands in this category.
redo-cmd-list	Remake the text and HTML files with the list of this
		  this category's commands.

clean		Remove all object files, their debugging versions, and
		  the man pages for the commands.
uninstall	Uninstall the object files, along with their HTML and troff
		  man pages.
END
