################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/numeric/weights/main.o 

C_SRCS += \
../src/bin/numeric/weights/main.c 

OBJS += \
./src/bin/numeric/weights/main.o 

C_DEPS += \
./src/bin/numeric/weights/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/numeric/weights/%.o: ../src/bin/numeric/weights/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


