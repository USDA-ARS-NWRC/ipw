################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/iadd/headers.c \
../src/bin/pixcalc/iadd/iadd.c \
../src/bin/pixcalc/iadd/main.c \
../src/bin/pixcalc/iadd/output.c \
../src/bin/pixcalc/iadd/parm.c 

OBJS += \
./src/bin/pixcalc/iadd/headers.o \
./src/bin/pixcalc/iadd/iadd.o \
./src/bin/pixcalc/iadd/main.o \
./src/bin/pixcalc/iadd/output.o \
./src/bin/pixcalc/iadd/parm.o 

C_DEPS += \
./src/bin/pixcalc/iadd/headers.d \
./src/bin/pixcalc/iadd/iadd.d \
./src/bin/pixcalc/iadd/main.d \
./src/bin/pixcalc/iadd/output.d \
./src/bin/pixcalc/iadd/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/iadd/%.o: ../src/bin/pixcalc/iadd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


