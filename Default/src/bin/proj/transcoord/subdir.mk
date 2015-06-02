################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/proj/transcoord/main.c \
../src/bin/proj/transcoord/transcoord.c 

OBJS += \
./src/bin/proj/transcoord/main.o \
./src/bin/proj/transcoord/transcoord.o 

C_DEPS += \
./src/bin/proj/transcoord/main.d \
./src/bin/proj/transcoord/transcoord.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/proj/transcoord/%.o: ../src/bin/proj/transcoord/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


