################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/scale/fast_scale.o \
../src/bin/pixcalc/scale/headers.o \
../src/bin/pixcalc/scale/main.o \
../src/bin/pixcalc/scale/output.o \
../src/bin/pixcalc/scale/parm.o \
../src/bin/pixcalc/scale/scale.o 

C_SRCS += \
../src/bin/pixcalc/scale/fast_scale.c \
../src/bin/pixcalc/scale/headers.c \
../src/bin/pixcalc/scale/main.c \
../src/bin/pixcalc/scale/output.c \
../src/bin/pixcalc/scale/parm.c \
../src/bin/pixcalc/scale/scale.c 

OBJS += \
./src/bin/pixcalc/scale/fast_scale.o \
./src/bin/pixcalc/scale/headers.o \
./src/bin/pixcalc/scale/main.o \
./src/bin/pixcalc/scale/output.o \
./src/bin/pixcalc/scale/parm.o \
./src/bin/pixcalc/scale/scale.o 

C_DEPS += \
./src/bin/pixcalc/scale/fast_scale.d \
./src/bin/pixcalc/scale/headers.d \
./src/bin/pixcalc/scale/main.d \
./src/bin/pixcalc/scale/output.d \
./src/bin/pixcalc/scale/parm.d \
./src/bin/pixcalc/scale/scale.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/scale/%.o: ../src/bin/pixcalc/scale/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


