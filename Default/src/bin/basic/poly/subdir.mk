################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/basic/poly/bresenham.c \
../src/bin/basic/poly/extern.c \
../src/bin/basic/poly/fill.c \
../src/bin/basic/poly/main.c \
../src/bin/basic/poly/poly.c 

OBJS += \
./src/bin/basic/poly/bresenham.o \
./src/bin/basic/poly/extern.o \
./src/bin/basic/poly/fill.o \
./src/bin/basic/poly/main.o \
./src/bin/basic/poly/poly.o 

C_DEPS += \
./src/bin/basic/poly/bresenham.d \
./src/bin/basic/poly/extern.d \
./src/bin/basic/poly/fill.d \
./src/bin/basic/poly/main.d \
./src/bin/basic/poly/poly.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/basic/poly/%.o: ../src/bin/basic/poly/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


