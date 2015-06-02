################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/deltaT/deltaT.c \
../src/bin/phys.spat/deltaT/headers.c \
../src/bin/phys.spat/deltaT/main.c \
../src/bin/phys.spat/deltaT/output.c \
../src/bin/phys.spat/deltaT/parm.c 

OBJS += \
./src/bin/phys.spat/deltaT/deltaT.o \
./src/bin/phys.spat/deltaT/headers.o \
./src/bin/phys.spat/deltaT/main.o \
./src/bin/phys.spat/deltaT/output.o \
./src/bin/phys.spat/deltaT/parm.o 

C_DEPS += \
./src/bin/phys.spat/deltaT/deltaT.d \
./src/bin/phys.spat/deltaT/headers.d \
./src/bin/phys.spat/deltaT/main.d \
./src/bin/phys.spat/deltaT/output.d \
./src/bin/phys.spat/deltaT/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/deltaT/%.o: ../src/bin/phys.spat/deltaT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


