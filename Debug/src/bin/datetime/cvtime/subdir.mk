################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/datetime/cvtime/cvtime.o \
../src/bin/datetime/cvtime/main.o 

C_SRCS += \
../src/bin/datetime/cvtime/cvtime.c \
../src/bin/datetime/cvtime/main.c 

OBJS += \
./src/bin/datetime/cvtime/cvtime.o \
./src/bin/datetime/cvtime/main.o 

C_DEPS += \
./src/bin/datetime/cvtime/cvtime.d \
./src/bin/datetime/cvtime/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/datetime/cvtime/%.o: ../src/bin/datetime/cvtime/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


