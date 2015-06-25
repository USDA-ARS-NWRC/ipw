################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/export/ipw2bin/headers.c \
../src/bin/export/ipw2bin/ipw2bin.c \
../src/bin/export/ipw2bin/main.c 

OBJS += \
./src/bin/export/ipw2bin/headers.o \
./src/bin/export/ipw2bin/ipw2bin.o \
./src/bin/export/ipw2bin/main.o 

C_DEPS += \
./src/bin/export/ipw2bin/headers.d \
./src/bin/export/ipw2bin/ipw2bin.d \
./src/bin/export/ipw2bin/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/export/ipw2bin/%.o: ../src/bin/export/ipw2bin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


