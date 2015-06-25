################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/import/text2bin/main.c 

OBJS += \
./src/bin/import/text2bin/main.o 

C_DEPS += \
./src/bin/import/text2bin/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/import/text2bin/%.o: ../src/bin/import/text2bin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


