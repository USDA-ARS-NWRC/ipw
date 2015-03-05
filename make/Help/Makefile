#  Makefile to make the help files for the various shared Makefiles

HELP_FILES := category script script_aux program program_aux aux_pgm \
	      library funcCategory funcCatManpg funcManpage \
	      manpage bin_dir lib_dir src_dir

HELP_FILES := $(foreach file, $(HELP_FILES), $(file).hlp)

.PHONY : all
all : $(HELP_FILES)

$(HELP_FILES) : %.hlp : %.sh
	printf "Possible targets for \"$(MAKE)\" command:\n\n" > $@
	sh $(@:.hlp=.sh) >> $@
	cat _help >> $@

program.hlp program_aux.hlp script.hlp script_aux.hlp : _command.sh
