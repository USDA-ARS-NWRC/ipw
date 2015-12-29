################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/import/netcdf2ipw/ffscanf.o \
../src/bin/import/netcdf2ipw/headers.o \
../src/bin/import/netcdf2ipw/main.o \
../src/bin/import/netcdf2ipw/newlqh.o \
../src/bin/import/netcdf2ipw/output.o \
../src/bin/import/netcdf2ipw/quick_import.o \
../src/bin/import/netcdf2ipw/text2ipw.o 

C_SRCS += \
../src/bin/import/netcdf2ipw/ffscanf.c \
../src/bin/import/netcdf2ipw/headers.c \
../src/bin/import/netcdf2ipw/main.c \
../src/bin/import/netcdf2ipw/newlqh.c \
../src/bin/import/netcdf2ipw/output.c \
../src/bin/import/netcdf2ipw/quick_import.c \
../src/bin/import/netcdf2ipw/read_nc_3D.c \
../src/bin/import/netcdf2ipw/text2ipw.c 

OBJS += \
./src/bin/import/netcdf2ipw/ffscanf.o \
./src/bin/import/netcdf2ipw/headers.o \
./src/bin/import/netcdf2ipw/main.o \
./src/bin/import/netcdf2ipw/newlqh.o \
./src/bin/import/netcdf2ipw/output.o \
./src/bin/import/netcdf2ipw/quick_import.o \
./src/bin/import/netcdf2ipw/read_nc_3D.o \
./src/bin/import/netcdf2ipw/text2ipw.o 

C_DEPS += \
./src/bin/import/netcdf2ipw/ffscanf.d \
./src/bin/import/netcdf2ipw/headers.d \
./src/bin/import/netcdf2ipw/main.d \
./src/bin/import/netcdf2ipw/newlqh.d \
./src/bin/import/netcdf2ipw/output.d \
./src/bin/import/netcdf2ipw/quick_import.d \
./src/bin/import/netcdf2ipw/read_nc_3D.d \
./src/bin/import/netcdf2ipw/text2ipw.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/netcdf2ipw/%.o: ../src/bin/import/netcdf2ipw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


