################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/rad.bas/topotherm/headers.c \
../src/bin/rad.bas/topotherm/main.c \
../src/bin/rad.bas/topotherm/newlqh.c \
../src/bin/rad.bas/topotherm/output.c \
../src/bin/rad.bas/topotherm/topotherm.c 

OBJS += \
./src/bin/rad.bas/topotherm/headers.o \
./src/bin/rad.bas/topotherm/main.o \
./src/bin/rad.bas/topotherm/newlqh.o \
./src/bin/rad.bas/topotherm/output.o \
./src/bin/rad.bas/topotherm/topotherm.o 

C_DEPS += \
./src/bin/rad.bas/topotherm/headers.d \
./src/bin/rad.bas/topotherm/main.d \
./src/bin/rad.bas/topotherm/newlqh.d \
./src/bin/rad.bas/topotherm/output.d \
./src/bin/rad.bas/topotherm/topotherm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/rad.bas/topotherm/%.o: ../src/bin/rad.bas/topotherm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


