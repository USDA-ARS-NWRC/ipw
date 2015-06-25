################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/import/msurface/geodist.c \
../src/bin/import/msurface/get_distance.c \
../src/bin/import/msurface/get_points.c \
../src/bin/import/msurface/headers.c \
../src/bin/import/msurface/interp.c \
../src/bin/import/msurface/main.c \
../src/bin/import/msurface/parm.c \
../src/bin/import/msurface/surface.c 

OBJS += \
./src/bin/import/msurface/geodist.o \
./src/bin/import/msurface/get_distance.o \
./src/bin/import/msurface/get_points.o \
./src/bin/import/msurface/headers.o \
./src/bin/import/msurface/interp.o \
./src/bin/import/msurface/main.o \
./src/bin/import/msurface/parm.o \
./src/bin/import/msurface/surface.o 

C_DEPS += \
./src/bin/import/msurface/geodist.d \
./src/bin/import/msurface/get_distance.d \
./src/bin/import/msurface/get_points.d \
./src/bin/import/msurface/headers.d \
./src/bin/import/msurface/interp.d \
./src/bin/import/msurface/main.d \
./src/bin/import/msurface/parm.d \
./src/bin/import/msurface/surface.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/msurface/%.o: ../src/bin/import/msurface/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


