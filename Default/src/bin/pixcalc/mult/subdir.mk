################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/mult/headers.c \
../src/bin/pixcalc/mult/main.c \
../src/bin/pixcalc/mult/mult.c \
../src/bin/pixcalc/mult/output.c \
../src/bin/pixcalc/mult/parm.c 

OBJS += \
./src/bin/pixcalc/mult/headers.o \
./src/bin/pixcalc/mult/main.o \
./src/bin/pixcalc/mult/mult.o \
./src/bin/pixcalc/mult/output.o \
./src/bin/pixcalc/mult/parm.o 

C_DEPS += \
./src/bin/pixcalc/mult/headers.d \
./src/bin/pixcalc/mult/main.d \
./src/bin/pixcalc/mult/mult.d \
./src/bin/pixcalc/mult/output.d \
./src/bin/pixcalc/mult/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/mult/%.o: ../src/bin/pixcalc/mult/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


