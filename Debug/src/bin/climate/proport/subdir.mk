################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/climate/proport/main.o \
../src/bin/climate/proport/proport.o 

C_SRCS += \
../src/bin/climate/proport/main.c \
../src/bin/climate/proport/proport.c 

OBJS += \
./src/bin/climate/proport/main.o \
./src/bin/climate/proport/proport.o 

C_DEPS += \
./src/bin/climate/proport/main.d \
./src/bin/climate/proport/proport.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/climate/proport/%.o: ../src/bin/climate/proport/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


