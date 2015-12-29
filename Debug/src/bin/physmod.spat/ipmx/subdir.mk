################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.spat/ipmx/get_sample.o \
../src/bin/physmod.spat/ipmx/headers.o \
../src/bin/physmod.spat/ipmx/ipmx.o \
../src/bin/physmod.spat/ipmx/main.o \
../src/bin/physmod.spat/ipmx/newlqh.o \
../src/bin/physmod.spat/ipmx/output.o 

C_SRCS += \
../src/bin/physmod.spat/ipmx/get_sample.c \
../src/bin/physmod.spat/ipmx/headers.c \
../src/bin/physmod.spat/ipmx/ipmx.c \
../src/bin/physmod.spat/ipmx/main.c \
../src/bin/physmod.spat/ipmx/newlqh.c \
../src/bin/physmod.spat/ipmx/output.c 

OBJS += \
./src/bin/physmod.spat/ipmx/get_sample.o \
./src/bin/physmod.spat/ipmx/headers.o \
./src/bin/physmod.spat/ipmx/ipmx.o \
./src/bin/physmod.spat/ipmx/main.o \
./src/bin/physmod.spat/ipmx/newlqh.o \
./src/bin/physmod.spat/ipmx/output.o 

C_DEPS += \
./src/bin/physmod.spat/ipmx/get_sample.d \
./src/bin/physmod.spat/ipmx/headers.d \
./src/bin/physmod.spat/ipmx/ipmx.d \
./src/bin/physmod.spat/ipmx/main.d \
./src/bin/physmod.spat/ipmx/newlqh.d \
./src/bin/physmod.spat/ipmx/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/ipmx/%.o: ../src/bin/physmod.spat/ipmx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


