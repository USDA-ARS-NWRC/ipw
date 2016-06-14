PGM := isnobal
DESCRIPTION := energy-balance snowmelt model with 2 snow layers

SRCS := assign_buffers.c buffers.c check_range.c check_units.c copy_image.c \
	e_m_image.c extract_data.c headers.c isnobal.c main.c newlqh.c \
	open_input.c output_image.c precip_event.c precip_hdrs.c read_data.c \
	snow_image.c temp_filename.c vars.c warn_range.c write_data.c

EXTRA_LIBS := snobal model

#EXTRA_CFLAGS := -fopenmp

include $(IPW)/make/command
