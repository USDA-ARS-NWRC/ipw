################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/header/mkwinh/main.c \
../src/bin/header/mkwinh/mkwinh.c 

OBJS += \
./src/bin/header/mkwinh/main.o \
./src/bin/header/mkwinh/mkwinh.o 

C_DEPS += \
./src/bin/header/mkwinh/main.d \
./src/bin/header/mkwinh/mkwinh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mkwinh/%.o: ../src/bin/header/mkwinh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


