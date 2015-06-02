################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/header/mkcrh/get_classes.c \
../src/bin/header/mkcrh/main.c \
../src/bin/header/mkcrh/mkcrh.c 

OBJS += \
./src/bin/header/mkcrh/get_classes.o \
./src/bin/header/mkcrh/main.o \
./src/bin/header/mkcrh/mkcrh.o 

C_DEPS += \
./src/bin/header/mkcrh/get_classes.d \
./src/bin/header/mkcrh/main.d \
./src/bin/header/mkcrh/mkcrh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mkcrh/%.o: ../src/bin/header/mkcrh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


