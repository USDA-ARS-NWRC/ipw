################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/lib/libimage/mask/open_mask.o 

C_SRCS += \
../src/lib/libimage/mask/open_mask.c 

OBJS += \
./src/lib/libimage/mask/open_mask.o 

C_DEPS += \
./src/lib/libimage/mask/open_mask.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libimage/mask/%.o: ../src/lib/libimage/mask/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


