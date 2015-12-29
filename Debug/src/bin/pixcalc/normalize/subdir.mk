################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/pixcalc/normalize/headers.o \
../src/bin/pixcalc/normalize/main.o \
../src/bin/pixcalc/normalize/normalize.o \
../src/bin/pixcalc/normalize/output.o \
../src/bin/pixcalc/normalize/parm.o 

C_SRCS += \
../src/bin/pixcalc/normalize/headers.c \
../src/bin/pixcalc/normalize/main.c \
../src/bin/pixcalc/normalize/normalize.c \
../src/bin/pixcalc/normalize/output.c \
../src/bin/pixcalc/normalize/parm.c 

OBJS += \
./src/bin/pixcalc/normalize/headers.o \
./src/bin/pixcalc/normalize/main.o \
./src/bin/pixcalc/normalize/normalize.o \
./src/bin/pixcalc/normalize/output.o \
./src/bin/pixcalc/normalize/parm.o 

C_DEPS += \
./src/bin/pixcalc/normalize/headers.d \
./src/bin/pixcalc/normalize/main.d \
./src/bin/pixcalc/normalize/normalize.d \
./src/bin/pixcalc/normalize/output.d \
./src/bin/pixcalc/normalize/parm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/normalize/%.o: ../src/bin/pixcalc/normalize/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


