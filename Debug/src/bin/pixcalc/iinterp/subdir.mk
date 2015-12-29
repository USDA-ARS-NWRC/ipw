################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/iinterp/get_points.o \
../src/bin/pixcalc/iinterp/headers.o \
../src/bin/pixcalc/iinterp/iinterp.o \
../src/bin/pixcalc/iinterp/lutximg.o \
../src/bin/pixcalc/iinterp/main.o \
../src/bin/pixcalc/iinterp/newlqh.o 

C_SRCS += \
../src/bin/pixcalc/iinterp/get_points.c \
../src/bin/pixcalc/iinterp/headers.c \
../src/bin/pixcalc/iinterp/iinterp.c \
../src/bin/pixcalc/iinterp/lutximg.c \
../src/bin/pixcalc/iinterp/main.c \
../src/bin/pixcalc/iinterp/newlqh.c 

OBJS += \
./src/bin/pixcalc/iinterp/get_points.o \
./src/bin/pixcalc/iinterp/headers.o \
./src/bin/pixcalc/iinterp/iinterp.o \
./src/bin/pixcalc/iinterp/lutximg.o \
./src/bin/pixcalc/iinterp/main.o \
./src/bin/pixcalc/iinterp/newlqh.o 

C_DEPS += \
./src/bin/pixcalc/iinterp/get_points.d \
./src/bin/pixcalc/iinterp/headers.d \
./src/bin/pixcalc/iinterp/iinterp.d \
./src/bin/pixcalc/iinterp/lutximg.d \
./src/bin/pixcalc/iinterp/main.d \
./src/bin/pixcalc/iinterp/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/iinterp/%.o: ../src/bin/pixcalc/iinterp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


