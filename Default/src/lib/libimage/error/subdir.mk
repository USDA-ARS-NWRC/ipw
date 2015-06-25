################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/libimage/error/ipw_errno.c \
../src/lib/libimage/error/ipw_strerr.c 

OBJS += \
./src/lib/libimage/error/ipw_errno.o \
./src/lib/libimage/error/ipw_strerr.o 

C_DEPS += \
./src/lib/libimage/error/ipw_errno.d \
./src/lib/libimage/error/ipw_strerr.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/libimage/error/%.o: ../src/lib/libimage/error/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


