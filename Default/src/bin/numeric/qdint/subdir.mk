################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/numeric/qdint/main.c 

OBJS += \
./src/bin/numeric/qdint/main.o 

C_DEPS += \
./src/bin/numeric/qdint/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/numeric/qdint/%.o: ../src/bin/numeric/qdint/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


