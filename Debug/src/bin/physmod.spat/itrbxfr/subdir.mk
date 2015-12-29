################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.spat/itrbxfr/bihvalid.o \
../src/bin/physmod.spat/itrbxfr/get_sample.o \
../src/bin/physmod.spat/itrbxfr/headers.o \
../src/bin/physmod.spat/itrbxfr/itrbxfr.o \
../src/bin/physmod.spat/itrbxfr/main.o \
../src/bin/physmod.spat/itrbxfr/newlqh.o \
../src/bin/physmod.spat/itrbxfr/output.o 

C_SRCS += \
../src/bin/physmod.spat/itrbxfr/bihvalid.c \
../src/bin/physmod.spat/itrbxfr/get_sample.c \
../src/bin/physmod.spat/itrbxfr/headers.c \
../src/bin/physmod.spat/itrbxfr/itrbxfr.c \
../src/bin/physmod.spat/itrbxfr/main.c \
../src/bin/physmod.spat/itrbxfr/newlqh.c \
../src/bin/physmod.spat/itrbxfr/output.c 

OBJS += \
./src/bin/physmod.spat/itrbxfr/bihvalid.o \
./src/bin/physmod.spat/itrbxfr/get_sample.o \
./src/bin/physmod.spat/itrbxfr/headers.o \
./src/bin/physmod.spat/itrbxfr/itrbxfr.o \
./src/bin/physmod.spat/itrbxfr/main.o \
./src/bin/physmod.spat/itrbxfr/newlqh.o \
./src/bin/physmod.spat/itrbxfr/output.o 

C_DEPS += \
./src/bin/physmod.spat/itrbxfr/bihvalid.d \
./src/bin/physmod.spat/itrbxfr/get_sample.d \
./src/bin/physmod.spat/itrbxfr/headers.d \
./src/bin/physmod.spat/itrbxfr/itrbxfr.d \
./src/bin/physmod.spat/itrbxfr/main.d \
./src/bin/physmod.spat/itrbxfr/newlqh.d \
./src/bin/physmod.spat/itrbxfr/output.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.spat/itrbxfr/%.o: ../src/bin/physmod.spat/itrbxfr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


