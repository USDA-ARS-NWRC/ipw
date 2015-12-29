################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/physmod.spat/isnobalp/assign_buffers.c \
../src/bin/physmod.spat/isnobalp/buffers.c \
../src/bin/physmod.spat/isnobalp/check_range.c \
../src/bin/physmod.spat/isnobalp/check_units.c \
../src/bin/physmod.spat/isnobalp/copy_image.c \
../src/bin/physmod.spat/isnobalp/e_m_image.c \
../src/bin/physmod.spat/isnobalp/extract_data.c \
../src/bin/physmod.spat/isnobalp/headers.c \
../src/bin/physmod.spat/isnobalp/isnobalp.c \
../src/bin/physmod.spat/isnobalp/main.c \
../src/bin/physmod.spat/isnobalp/newlqh.c \
../src/bin/physmod.spat/isnobalp/open_input.c \
../src/bin/physmod.spat/isnobalp/output_image.c \
../src/bin/physmod.spat/isnobalp/precip_event.c \
../src/bin/physmod.spat/isnobalp/precip_hdrs.c \
../src/bin/physmod.spat/isnobalp/read_data.c \
../src/bin/physmod.spat/isnobalp/snow_image.c \
../src/bin/physmod.spat/isnobalp/temp_filename.c \
../src/bin/physmod.spat/isnobalp/vars.c \
../src/bin/physmod.spat/isnobalp/write_data.c 

OBJS += \
./src/bin/physmod.spat/isnobalp/assign_buffers.o \
./src/bin/physmod.spat/isnobalp/buffers.o \
./src/bin/physmod.spat/isnobalp/check_range.o \
./src/bin/physmod.spat/isnobalp/check_units.o \
./src/bin/physmod.spat/isnobalp/copy_image.o \
./src/bin/physmod.spat/isnobalp/e_m_image.o \
./src/bin/physmod.spat/isnobalp/extract_data.o \
./src/bin/physmod.spat/isnobalp/headers.o \
./src/bin/physmod.spat/isnobalp/isnobalp.o \
./src/bin/physmod.spat/isnobalp/main.o \
./src/bin/physmod.spat/isnobalp/newlqh.o \
./src/bin/physmod.spat/isnobalp/open_input.o \
./src/bin/physmod.spat/isnobalp/output_image.o \
./src/bin/physmod.spat/isnobalp/precip_event.o \
./src/bin/physmod.spat/isnobalp/precip_hdrs.o \
./src/bin/physmod.spat/isnobalp/read_data.o \
./src/bin/physmod.spat/isnobalp/snow_image.o \
./src/bin/physmod.spat/isnobalp/temp_filename.o \
./src/bin/physmod.spat/isnobalp/vars.o \
./src/bin/physmod.spat/isnobalp/write_data.o 

C_DEPS += \
./src/bin/physmod.spat/isnobalp/assign_buffers.d \
./src/bin/physmod.spat/isnobalp/buffers.d \
./src/bin/physmod.spat/isnobalp/check_range.d \
./src/bin/physmod.spat/isnobalp/check_units.d \
./src/bin/physmod.spat/isnobalp/copy_image.d \
./src/bin/physmod.spat/isnobalp/e_m_image.d \
./src/bin/physmod.spat/isnobalp/extract_data.d \
./src/bin/physmod.spat/isnobalp/headers.d \
./src/bin/physmod.spat/isnobalp/isnobalp.d \
./src/bin/physmod.spat/isnobalp/main.d \
./src/bin/physmod.spat/isnobalp/newlqh.d \
./src/bin/physmod.spat/isnobalp/open_input.d \
./src/bin/physmod.spat/isnobalp/output_image.d \
./src/bin/physmod.spat/isnobalp/precip_event.d \
./src/bin/physmod.spat/isnobalp/precip_hdrs.d \
./src/bin/physmod.spat/isnobalp/read_data.d \
./src/bin/physmod.spat/isnobalp/snow_image.d \
./src/bin/physmod.spat/isnobalp/temp_filename.d \
./src/bin/physmod.spat/isnobalp/vars.d \
./src/bin/physmod.spat/isnobalp/write_data.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/isnobalp/%.o: ../src/bin/physmod.spat/isnobalp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


