################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.spat/isnobal/assign_buffers.o \
../src/bin/physmod.spat/isnobal/buffers.o \
../src/bin/physmod.spat/isnobal/check_range.o \
../src/bin/physmod.spat/isnobal/check_units.o \
../src/bin/physmod.spat/isnobal/copy_image.o \
../src/bin/physmod.spat/isnobal/e_m_image.o \
../src/bin/physmod.spat/isnobal/extract_data.o \
../src/bin/physmod.spat/isnobal/headers.o \
../src/bin/physmod.spat/isnobal/isnobal.o \
../src/bin/physmod.spat/isnobal/main.o \
../src/bin/physmod.spat/isnobal/newlqh.o \
../src/bin/physmod.spat/isnobal/open_input.o \
../src/bin/physmod.spat/isnobal/output_image.o \
../src/bin/physmod.spat/isnobal/precip_event.o \
../src/bin/physmod.spat/isnobal/precip_hdrs.o \
../src/bin/physmod.spat/isnobal/read_data.o \
../src/bin/physmod.spat/isnobal/snow_image.o \
../src/bin/physmod.spat/isnobal/temp_filename.o \
../src/bin/physmod.spat/isnobal/vars.o \
../src/bin/physmod.spat/isnobal/write_data.o 

C_SRCS += \
../src/bin/physmod.spat/isnobal/assign_buffers.c \
../src/bin/physmod.spat/isnobal/buffers.c \
../src/bin/physmod.spat/isnobal/check_range.c \
../src/bin/physmod.spat/isnobal/check_units.c \
../src/bin/physmod.spat/isnobal/copy_image.c \
../src/bin/physmod.spat/isnobal/e_m_image.c \
../src/bin/physmod.spat/isnobal/extract_data.c \
../src/bin/physmod.spat/isnobal/headers.c \
../src/bin/physmod.spat/isnobal/isnobal.c \
../src/bin/physmod.spat/isnobal/main.c \
../src/bin/physmod.spat/isnobal/newlqh.c \
../src/bin/physmod.spat/isnobal/open_input.c \
../src/bin/physmod.spat/isnobal/output_image.c \
../src/bin/physmod.spat/isnobal/precip_event.c \
../src/bin/physmod.spat/isnobal/precip_hdrs.c \
../src/bin/physmod.spat/isnobal/read_data.c \
../src/bin/physmod.spat/isnobal/snow_image.c \
../src/bin/physmod.spat/isnobal/temp_filename.c \
../src/bin/physmod.spat/isnobal/vars.c \
../src/bin/physmod.spat/isnobal/write_data.c 

OBJS += \
./src/bin/physmod.spat/isnobal/assign_buffers.o \
./src/bin/physmod.spat/isnobal/buffers.o \
./src/bin/physmod.spat/isnobal/check_range.o \
./src/bin/physmod.spat/isnobal/check_units.o \
./src/bin/physmod.spat/isnobal/copy_image.o \
./src/bin/physmod.spat/isnobal/e_m_image.o \
./src/bin/physmod.spat/isnobal/extract_data.o \
./src/bin/physmod.spat/isnobal/headers.o \
./src/bin/physmod.spat/isnobal/isnobal.o \
./src/bin/physmod.spat/isnobal/main.o \
./src/bin/physmod.spat/isnobal/newlqh.o \
./src/bin/physmod.spat/isnobal/open_input.o \
./src/bin/physmod.spat/isnobal/output_image.o \
./src/bin/physmod.spat/isnobal/precip_event.o \
./src/bin/physmod.spat/isnobal/precip_hdrs.o \
./src/bin/physmod.spat/isnobal/read_data.o \
./src/bin/physmod.spat/isnobal/snow_image.o \
./src/bin/physmod.spat/isnobal/temp_filename.o \
./src/bin/physmod.spat/isnobal/vars.o \
./src/bin/physmod.spat/isnobal/write_data.o 

C_DEPS += \
./src/bin/physmod.spat/isnobal/assign_buffers.d \
./src/bin/physmod.spat/isnobal/buffers.d \
./src/bin/physmod.spat/isnobal/check_range.d \
./src/bin/physmod.spat/isnobal/check_units.d \
./src/bin/physmod.spat/isnobal/copy_image.d \
./src/bin/physmod.spat/isnobal/e_m_image.d \
./src/bin/physmod.spat/isnobal/extract_data.d \
./src/bin/physmod.spat/isnobal/headers.d \
./src/bin/physmod.spat/isnobal/isnobal.d \
./src/bin/physmod.spat/isnobal/main.d \
./src/bin/physmod.spat/isnobal/newlqh.d \
./src/bin/physmod.spat/isnobal/open_input.d \
./src/bin/physmod.spat/isnobal/output_image.d \
./src/bin/physmod.spat/isnobal/precip_event.d \
./src/bin/physmod.spat/isnobal/precip_hdrs.d \
./src/bin/physmod.spat/isnobal/read_data.d \
./src/bin/physmod.spat/isnobal/snow_image.d \
./src/bin/physmod.spat/isnobal/temp_filename.d \
./src/bin/physmod.spat/isnobal/vars.d \
./src/bin/physmod.spat/isnobal/write_data.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/isnobal/%.o: ../src/bin/physmod.spat/isnobal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


