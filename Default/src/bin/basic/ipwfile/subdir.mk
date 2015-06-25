################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/basic/ipwfile/headers.c \
../src/bin/basic/ipwfile/main.c \
../src/bin/basic/ipwfile/parm.c 

OBJS += \
./src/bin/basic/ipwfile/headers.o \
./src/bin/basic/ipwfile/main.o \
./src/bin/basic/ipwfile/parm.o 

C_DEPS += \
./src/bin/basic/ipwfile/headers.d \
./src/bin/basic/ipwfile/main.d \
./src/bin/basic/ipwfile/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/basic/ipwfile/%.o: ../src/bin/basic/ipwfile/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


