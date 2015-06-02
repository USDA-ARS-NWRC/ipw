################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/phys.spat/ipott/headers.c \
../src/bin/phys.spat/ipott/ipott.c \
../src/bin/phys.spat/ipott/main.c 

OBJS += \
./src/bin/phys.spat/ipott/headers.o \
./src/bin/phys.spat/ipott/ipott.o \
./src/bin/phys.spat/ipott/main.o 

C_DEPS += \
./src/bin/phys.spat/ipott/headers.d \
./src/bin/phys.spat/ipott/ipott.d \
./src/bin/phys.spat/ipott/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/phys.spat/ipott/%.o: ../src/bin/phys.spat/ipott/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


