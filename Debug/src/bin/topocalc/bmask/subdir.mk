################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/topocalc/bmask/fileRead.o \
../src/bin/topocalc/bmask/fileWrite.o \
../src/bin/topocalc/bmask/main.o 

C_SRCS += \
../src/bin/topocalc/bmask/fileRead.c \
../src/bin/topocalc/bmask/fileWrite.c \
../src/bin/topocalc/bmask/main.c 

OBJS += \
./src/bin/topocalc/bmask/fileRead.o \
./src/bin/topocalc/bmask/fileWrite.o \
./src/bin/topocalc/bmask/main.o 

C_DEPS += \
./src/bin/topocalc/bmask/fileRead.d \
./src/bin/topocalc/bmask/fileWrite.d \
./src/bin/topocalc/bmask/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/topocalc/bmask/%.o: ../src/bin/topocalc/bmask/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


