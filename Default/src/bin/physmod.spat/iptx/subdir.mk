################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/physmod.spat/iptx/get_sample.c \
../src/bin/physmod.spat/iptx/headers.c \
../src/bin/physmod.spat/iptx/iptx.c \
../src/bin/physmod.spat/iptx/main.c \
../src/bin/physmod.spat/iptx/newlqh.c \
../src/bin/physmod.spat/iptx/output.c 

OBJS += \
./src/bin/physmod.spat/iptx/get_sample.o \
./src/bin/physmod.spat/iptx/headers.o \
./src/bin/physmod.spat/iptx/iptx.o \
./src/bin/physmod.spat/iptx/main.o \
./src/bin/physmod.spat/iptx/newlqh.o \
./src/bin/physmod.spat/iptx/output.o 

C_DEPS += \
./src/bin/physmod.spat/iptx/get_sample.d \
./src/bin/physmod.spat/iptx/headers.d \
./src/bin/physmod.spat/iptx/iptx.d \
./src/bin/physmod.spat/iptx/main.d \
./src/bin/physmod.spat/iptx/newlqh.d \
./src/bin/physmod.spat/iptx/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/iptx/%.o: ../src/bin/physmod.spat/iptx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


