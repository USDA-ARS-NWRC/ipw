################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/export/bin2text/main.o 

C_SRCS += \
../src/bin/export/bin2text/main.c 

OBJS += \
./src/bin/export/bin2text/main.o 

C_DEPS += \
./src/bin/export/bin2text/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/bin2text/%.o: ../src/bin/export/bin2text/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


