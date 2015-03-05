CMD_TYPE := User
DIRS := datetime numeric climate import export header basic lq transform \
	pixcalc stats classify topocalc proj phys.pt phys.spat \
	physmod.pt physmod.spat rad.pt rad.bas rad.glb misc

INDEX_FILE := Index.html

include $(IPW)/make/bin_dir

index : $(INDEX_FILE)

#  The path to the top of the Web directory for a bin_dir doesn't work
#  for the head and address of the index file, so we need to redefine
#  the macro "path_to_top".

$(INDEX_FILE) :
	@printf "%%define(ipw_href_path,../)\n%%Title(\"Index of IPW Commands \")\n" | $(IPW_MACRO_DOC) - >$@
	@$(IPW)/make/bin/makeIndex $(DIRS) >> $@
	@printf "%%define(ipw_href_path,../)\n" | $(IPW_MACRO_DOC) - $(DOC_SRC_DIR)/PageBottom2 >> $@
