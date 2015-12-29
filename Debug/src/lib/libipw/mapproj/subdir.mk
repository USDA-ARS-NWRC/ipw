################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libipw/mapproj/get_proj_id.o \
../src/lib/libipw/mapproj/get_proj_name.o \
../src/lib/libipw/mapproj/get_proj_units.o \
../src/lib/libipw/mapproj/pack_dms.o \
../src/lib/libipw/mapproj/read_proj.o \
../src/lib/libipw/mapproj/unpack_dms.o 

C_SRCS += \
../src/lib/libipw/mapproj/get_proj_id.c \
../src/lib/libipw/mapproj/get_proj_name.c \
../src/lib/libipw/mapproj/get_proj_units.c \
../src/lib/libipw/mapproj/pack_dms.c \
../src/lib/libipw/mapproj/read_proj.c \
../src/lib/libipw/mapproj/unpack_dms.c 

OBJS += \
./src/lib/libipw/mapproj/get_proj_id.o \
./src/lib/libipw/mapproj/get_proj_name.o \
./src/lib/libipw/mapproj/get_proj_units.o \
./src/lib/libipw/mapproj/pack_dms.o \
./src/lib/libipw/mapproj/read_proj.o \
./src/lib/libipw/mapproj/unpack_dms.o 

C_DEPS += \
./src/lib/libipw/mapproj/get_proj_id.d \
./src/lib/libipw/mapproj/get_proj_name.d \
./src/lib/libipw/mapproj/get_proj_units.d \
./src/lib/libipw/mapproj/pack_dms.d \
./src/lib/libipw/mapproj/read_proj.d \
./src/lib/libipw/mapproj/unpack_dms.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libipw/mapproj/%.o: ../src/lib/libipw/mapproj/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


