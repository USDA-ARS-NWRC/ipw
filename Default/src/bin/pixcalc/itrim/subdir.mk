################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/itrim/headers.c \
../src/bin/pixcalc/itrim/main.c \
../src/bin/pixcalc/itrim/output.c \
../src/bin/pixcalc/itrim/parm.c \
../src/bin/pixcalc/itrim/scale.c 

OBJS += \
./src/bin/pixcalc/itrim/headers.o \
./src/bin/pixcalc/itrim/main.o \
./src/bin/pixcalc/itrim/output.o \
./src/bin/pixcalc/itrim/parm.o \
./src/bin/pixcalc/itrim/scale.o 

C_DEPS += \
./src/bin/pixcalc/itrim/headers.d \
./src/bin/pixcalc/itrim/main.d \
./src/bin/pixcalc/itrim/output.d \
./src/bin/pixcalc/itrim/parm.d \
./src/bin/pixcalc/itrim/scale.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/itrim/%.o: ../src/bin/pixcalc/itrim/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


