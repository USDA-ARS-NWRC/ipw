################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.pt/humidx/main.o 

C_SRCS += \
../src/bin/physmod.pt/humidx/main.c 

OBJS += \
./src/bin/physmod.pt/humidx/main.o 

C_DEPS += \
./src/bin/physmod.pt/humidx/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.pt/humidx/%.o: ../src/bin/physmod.pt/humidx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


