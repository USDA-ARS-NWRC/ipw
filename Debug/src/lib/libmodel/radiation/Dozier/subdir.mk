################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libmodel/radiation/Dozier/planck.c 

OBJS += \
./src/lib/libmodel/radiation/Dozier/planck.o 

C_DEPS += \
./src/lib/libmodel/radiation/Dozier/planck.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libmodel/radiation/Dozier/%.o: ../src/lib/libmodel/radiation/Dozier/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


