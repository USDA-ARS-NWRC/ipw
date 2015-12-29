################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/header/mklqh/main.o \
../src/bin/header/mklqh/mklqh.o 

C_SRCS += \
../src/bin/header/mklqh/main.c \
../src/bin/header/mklqh/mklqh.c 

OBJS += \
./src/bin/header/mklqh/main.o \
./src/bin/header/mklqh/mklqh.o 

C_DEPS += \
./src/bin/header/mklqh/main.d \
./src/bin/header/mklqh/mklqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/header/mklqh/%.o: ../src/bin/header/mklqh/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


