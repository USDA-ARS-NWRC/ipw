################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/transform/window/cvt_wspec.c \
../src/bin/transform/window/fix_wspec.c \
../src/bin/transform/window/main.c \
../src/bin/transform/window/window.c 

OBJS += \
./src/bin/transform/window/cvt_wspec.o \
./src/bin/transform/window/fix_wspec.o \
./src/bin/transform/window/main.o \
./src/bin/transform/window/window.o 

C_DEPS += \
./src/bin/transform/window/cvt_wspec.d \
./src/bin/transform/window/fix_wspec.d \
./src/bin/transform/window/main.d \
./src/bin/transform/window/window.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/transform/window/%.o: ../src/bin/transform/window/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


