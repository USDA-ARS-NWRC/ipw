################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/topocalc/pmask/fileRead.c \
../src/bin/topocalc/pmask/fileWrite.c \
../src/bin/topocalc/pmask/main.c 

OBJS += \
./src/bin/topocalc/pmask/fileRead.o \
./src/bin/topocalc/pmask/fileWrite.o \
./src/bin/topocalc/pmask/main.o 

C_DEPS += \
./src/bin/topocalc/pmask/fileRead.d \
./src/bin/topocalc/pmask/fileWrite.d \
./src/bin/topocalc/pmask/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/pmask/%.o: ../src/bin/topocalc/pmask/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


