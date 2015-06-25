################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.bas/elevrad/data.c \
../src/bin/rad.bas/elevrad/elevrad.c \
../src/bin/rad.bas/elevrad/init_tau.c \
../src/bin/rad.bas/elevrad/main.c \
../src/bin/rad.bas/elevrad/radtbls.c 

OBJS += \
./src/bin/rad.bas/elevrad/data.o \
./src/bin/rad.bas/elevrad/elevrad.o \
./src/bin/rad.bas/elevrad/init_tau.o \
./src/bin/rad.bas/elevrad/main.o \
./src/bin/rad.bas/elevrad/radtbls.o 

C_DEPS += \
./src/bin/rad.bas/elevrad/data.d \
./src/bin/rad.bas/elevrad/elevrad.d \
./src/bin/rad.bas/elevrad/init_tau.d \
./src/bin/rad.bas/elevrad/main.d \
./src/bin/rad.bas/elevrad/radtbls.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/elevrad/%.o: ../src/bin/rad.bas/elevrad/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


