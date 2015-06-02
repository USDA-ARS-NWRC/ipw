################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/transform/transpose/main.c \
../src/bin/transform/transpose/transpose.c \
../src/bin/transform/transpose/ximg.c 

OBJS += \
./src/bin/transform/transpose/main.o \
./src/bin/transform/transpose/transpose.o \
./src/bin/transform/transpose/ximg.o 

C_DEPS += \
./src/bin/transform/transpose/main.d \
./src/bin/transform/transpose/transpose.d \
./src/bin/transform/transpose/ximg.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/transpose/%.o: ../src/bin/transform/transpose/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


